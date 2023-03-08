#include "Dungeon.h"
#include "UObject/ConstructorHelpers.h"

#define LOCTEXT_NAMESPACE "DungeoneerGamePlugin"

ADungeon::ADungeon()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");

	static ConstructorHelpers::FObjectFinder<UStaticMesh> quad(TEXT("/DungeoneerGame/dungeon-quad.dungeon-quad"));
	static ConstructorHelpers::FObjectFinder<UMaterial> floor(TEXT("/DungeoneerGame/basic-floor-material.basic-floor-material"));
	static ConstructorHelpers::FObjectFinder<UMaterial> wall(TEXT("/DungeoneerGame/basic-wall-material.basic-wall-material"));
	static ConstructorHelpers::FObjectFinder<UMaterial> ceiling(TEXT("/DungeoneerGame/basic-ceiling-material.basic-ceiling-material"));

	DungeonQuad = quad.Object;

	FDungeonModel WallTemplate;
	WallTemplate.Mesh = DungeonQuad;
	WallTemplate.Materials = { wall.Object };
	DungeonPalette.Models.Emplace("DEFAULT_WALL", WallTemplate);

	FDungeonModel FloorTemplate;
	FloorTemplate.Mesh = DungeonQuad;
	FloorTemplate.Materials = { floor.Object };
	DungeonPalette.Models.Emplace("DEFAULT_FLOOR", FloorTemplate);

	FDungeonModel CeilingTemplate;
	CeilingTemplate.Mesh = DungeonQuad;
	CeilingTemplate.Materials = { ceiling.Object };
	DungeonPalette.Models.Emplace("DEFAULT_CEILING", CeilingTemplate);

	// Editor Materials, exclude in non-editor builds?
	static ConstructorHelpers::FObjectFinder<UMaterial> selection(TEXT("/DungeoneerGame/selection-border-material.selection-border-material"));
	SelectionMaterial = selection.Object;

	static ConstructorHelpers::FObjectFinder<UMaterial> plusIcon(TEXT("/DungeoneerGame/plus-icon-material.plus-icon-material"));
	PlusIconMaterial = plusIcon.Object;
}

void ADungeon::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (Tiles.Num() == 0)
		CreateTile(FIntVector(0,0,0));

	RegenerateTiles();
}

void ADungeon::CreateTile(FIntVector TilePoint)
{
	if (Tiles.Contains(TilePoint)) return;
	
	Modify();
	
	FDungeonTile NewTile = FDungeonTile();
	NewTile.SegmentModels[(int)EDungeonSegment::NORTH] = "DEFAULT_WALL";
	NewTile.SegmentModels[(int)EDungeonSegment::EAST] = "DEFAULT_WALL";
	NewTile.SegmentModels[(int)EDungeonSegment::SOUTH] = "DEFAULT_WALL";
	NewTile.SegmentModels[(int)EDungeonSegment::WEST] = "DEFAULT_WALL";
	NewTile.SegmentModels[(int)EDungeonSegment::DOWN] = "DEFAULT_FLOOR";
	NewTile.SegmentModels[(int)EDungeonSegment::UP] = "DEFAULT_CEILING";
	
	Tiles.Emplace(TilePoint, NewTile);
	
	RegenerateTiles();
}

void ADungeon::DeleteTile(FIntVector TilePoint)
{
	if (!Tiles.Contains(TilePoint)) return;
	
	Modify();
	Tiles.Remove(TilePoint);

	RegenerateTiles();
}

void ADungeon::SetSegmentTemplate(FIntVector TilePoint, EDungeonSegment Segment, FName Template)
{
	if (!Tiles.Contains(TilePoint)) return;
	
	Modify();
	Tiles[TilePoint].SegmentModels[(int)Segment] = Template;
	
	RegenerateTiles();
}

void ADungeon::AddTileGameplayTag(FIntVector TilePoint, FGameplayTag tag)
{
	if (!Tiles.Contains(TilePoint)) return;
	Tiles[TilePoint].Tags.AddTag(tag);
	OnTileTagsChanged.Broadcast(this, TilePoint);
}

void ADungeon::RemoveTileGameplayTag(FIntVector TilePoint, FGameplayTag tag)
{
	if (!Tiles.Contains(TilePoint)) return;
	Tiles[TilePoint].Tags.RemoveTag(tag);
	OnTileTagsChanged.Broadcast(this, TilePoint);
}

void ADungeon::RegenerateTiles()
{
	TArray<UActorComponent*> CurrentISMCs;
	GetComponents(UInstancedStaticMeshComponent::StaticClass(), CurrentISMCs);
	
	TSet<UInstancedStaticMeshComponent*> UnusedISMCs;
	for (int i=0; i < CurrentISMCs.Num(); i++)
	{
		UInstancedStaticMeshComponent* ISMC = Cast<UInstancedStaticMeshComponent>(CurrentISMCs[i]);
		if (!ISMC) continue;
		ISMC->bHasPerInstanceHitProxies = true;
		UnusedISMCs.Add(ISMC);
	}
	
	TMap<FName, FDungeonBatchedInstance> BatchedInstances;
	
	TArray<FIntVector> TilePoints;
	Tiles.GetKeys(TilePoints);
	for (int i=0; i < TilePoints.Num(); i++)
	{
		FDungeonTile Tile = Tiles[TilePoints[i]];

		// add all the segments to the batch.
		for (int s=0; s < DUNGEON_SEGMENT_COUNT; s++)
		{
			// if there is an opening in the direction, dont render that segment.
			if (Tiles.Contains(TilePoints[i] + DUNGEON_SEGMENT_OFFSETS[s]))
				continue;

			if (!BatchedInstances.Contains(Tile.SegmentModels[s]))
				BatchedInstances.Emplace(Tile.SegmentModels[s], FDungeonBatchedInstance());

			FTransform Transform = FTransform(
				DUNGEON_SEGMENT_ROTATIONS[s] + Tile.SegmentRotation[s],
				FVector(
					TilePoints[i].X * Scale,
					TilePoints[i].Y * Scale,
					TilePoints[i].Z * Scale + (Scale/2)),
				FVector(Scale/100 + 0.001f, Scale/100 + 0.001f, Scale/100 + 0.001f));

			FDungeonModel Model = DungeonPalette.Models.FindRef(Tile.SegmentModels[s]);
			BatchedInstances[Tile.SegmentModels[s]].Transforms.Emplace(Transform);
			BatchedInstances[Tile.SegmentModels[s]].TilePoints.Emplace(TilePoints[i]);
			BatchedInstances[Tile.SegmentModels[s]].Segments.Emplace(s);
		}
		OnRefreshTile.Broadcast(this, TilePoints[i]);
	}

	// take all the batches and add the instances!
	TArray<FName> Templates;
	BatchedInstances.GetKeys(Templates);
	for (int i=0; i < Templates.Num(); i++)
	{
		FDungeonBatchedInstance instance = BatchedInstances[Templates[i]];
		UInstancedStaticMeshComponent* ISMC = GetInstancedMeshComponent(Templates[i]);
		if (!ISMC) continue;
		ISMC->ClearInstances();
		ISMC->AddInstances(instance.Transforms, false);
		// pack in custom data that represents the tile point and segment type.
		for (int e=0; e < instance.Transforms.Num(); e++)
		{
			ISMC->SetCustomData(e, {
				(float)instance.TilePoints[e].X,
				(float)instance.TilePoints[e].Y,
				(float)instance.TilePoints[e].Z,
				(float)instance.Segments[e]
			});
		} 
		// was used, so now its not unused.
		UnusedISMCs.Remove(ISMC);
	}

	// find the removed ISMCs and remove the component and dereference them in the maps.
	TArray<UInstancedStaticMeshComponent*> RemovedISMCs = UnusedISMCs.Array();
	for (int i=0; i < RemovedISMCs.Num(); i++)
	{
		FName ModelName = ISMCValues.FindRef(RemovedISMCs[i]);
		ISMCValues.Remove(RemovedISMCs[i]);
		ISMCs.Remove(ModelName);

		RemovedISMCs[i]->UnregisterComponent();
		RemovedISMCs[i]->DestroyComponent();
	}

	// step all the ISCMs and refresh there model/materials.
	// this is done encase a mesh or material changed in a template.
	TArray<FName> AllTemplates;
	ISMCs.GetKeys(AllTemplates);
	for (int i=0; i < AllTemplates.Num(); i++)
	{
		FName TemplateName = AllTemplates[i];
		FDungeonModel Template = DungeonPalette.Models.FindRef(TemplateName);
		UInstancedStaticMeshComponent* ISMC = ISMCs.FindRef(TemplateName);
		if (ISMC)
		{
			ISMC->SetStaticMesh(Template.Mesh);
			for (int m=0; m < Template.Materials.Num(); m++)
				ISMC->SetMaterial(m, Template.Materials[m]);
		}
	}
	OnGenerated.Broadcast(this);
}

UInstancedStaticMeshComponent* ADungeon::GetInstancedMeshComponent(FName TemplateName)
{
	if (!DungeonPalette.Models.Contains(TemplateName)) return NULL;
	FDungeonModel Template = DungeonPalette.Models.FindRef(TemplateName);
	UInstancedStaticMeshComponent* ISMC = ISMCs.FindRef(TemplateName);
	if (!ISMC)
	{
		ISMC = NewObject<UInstancedStaticMeshComponent>(this);
		ISMC->bHasPerInstanceHitProxies = true;
		ISMC->NumCustomDataFloats = 4; // X,Y,Z,SEGMENT, if segment is negative, its a custom model index. remove the neg.
		ISMC->HitProxyPriority = EHitProxyPriority::HPP_UI;
		AddInstanceComponent(ISMC);
		ISMC->RegisterComponent();
		ISMC->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		
		ISMC->SetStaticMesh(Template.Mesh);
		for (int i=0; i < Template.Materials.Num(); i++)
			ISMC->SetMaterial(i, Template.Materials[i]);
		
		ISMCs.Emplace(TemplateName, ISMC);
		ISMCValues.Emplace(ISMC, TemplateName);
	}
	return ISMC;
}

#undef LOCTEXT_NAMESPACE