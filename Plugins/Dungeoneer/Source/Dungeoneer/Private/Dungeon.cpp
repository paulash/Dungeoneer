#include "Dungeon.h"
#include "UObject/ConstructorHelpers.h"

#define LOCTEXT_NAMESPACE "FDungeoneerEdModeToolkit"

ADungeon::ADungeon()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");

	static ConstructorHelpers::FObjectFinder<UStaticMesh> quad(TEXT("/Dungeoneer/dungeon-quad.dungeon-quad"));
	static ConstructorHelpers::FObjectFinder<UMaterial> floor(TEXT("/Dungeoneer/basic-floor-material.basic-floor-material"));
	static ConstructorHelpers::FObjectFinder<UMaterial> wall(TEXT("/Dungeoneer/basic-wall-material.basic-wall-material"));
	static ConstructorHelpers::FObjectFinder<UMaterial> ceiling(TEXT("/Dungeoneer/basic-ceiling-material.basic-ceiling-material"));

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

	SelectedTemplate = "DEFAULT_WALL";

	// Editor Materials, exclude in non-editor builds?
	static ConstructorHelpers::FObjectFinder<UMaterial> selection(TEXT("/Dungeoneer/selection-border-material.selection-border-material"));
	SelectionMaterial = selection.Object;

	static ConstructorHelpers::FObjectFinder<UMaterial> plusIcon(TEXT("/Dungeoneer/plus-icon-material.plus-icon-material"));
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

#ifdef WITH_EDITOR
	GEditor->BeginTransaction(LOCTEXT("AddTileTransactionName", "Add Tile"));
#endif
	Modify();
	
	FDungeonTile NewTile = FDungeonTile();
	NewTile.SegmentModels[(int)EDungeonDirection::NORTH] = "DEFAULT_WALL";
	NewTile.SegmentModels[(int)EDungeonDirection::EAST] = "DEFAULT_WALL";
	NewTile.SegmentModels[(int)EDungeonDirection::SOUTH] = "DEFAULT_WALL";
	NewTile.SegmentModels[(int)EDungeonDirection::WEST] = "DEFAULT_WALL";
	NewTile.SegmentModels[(int)EDungeonDirection::DOWN] = "DEFAULT_FLOOR";
	NewTile.SegmentModels[(int)EDungeonDirection::UP] = "DEFAULT_CEILING";
	
	Tiles.Emplace(TilePoint, NewTile);

#ifdef WITH_EDITOR
	GEditor->EndTransaction();
#endif
	
	RegenerateTiles();
}

void ADungeon::DeleteTile(FIntVector TilePoint)
{
	if (!Tiles.Contains(TilePoint)) return;

#ifdef WITH_EDITOR
	GEditor->BeginTransaction(LOCTEXT("DeleteTileTransactionName", "Delete Tile"));
#endif
	Modify();
	Tiles.Remove(TilePoint);
#ifdef WITH_EDITOR
	GEditor->EndTransaction();
#endif
	
	RegenerateTiles();
}

void ADungeon::SetSegmentTemplate(FIntVector TilePoint, EDungeonDirection Segment, FName Template)
{
	if (!Tiles.Contains(TilePoint)) return;

#ifdef WITH_EDITOR
	GEditor->BeginTransaction(LOCTEXT("PaintTileTransactionName", "Paint Tile"));
#endif
	Modify();
	Tiles[TilePoint].SegmentModels[(int)Segment] = Template;
#ifdef WITH_EDITOR
	GEditor->EndTransaction();
#endif
	
	RegenerateTiles();
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
		UnusedISMCs.Add(ISMC);
	}
	
	TMap<FName, TArray<FTransform>> BatchedInstances;
	
	TArray<FIntVector> TilePoints;
	Tiles.GetKeys(TilePoints);
	for (int i=0; i < TilePoints.Num(); i++)
	{
		FDungeonTile Tile = Tiles[TilePoints[i]];

		// add all the segments to the batch.
		for (int s=0; s < DUNGEON_SEGMENT_COUNT; s++)
		{
			// if there is an opening in the direction, dont render that segment.
			if (Tiles.Contains(TilePoints[i] + DUNGEON_DIRECTIONS[s]))
				continue;

			if (!BatchedInstances.Contains(Tile.SegmentModels[s]))
				BatchedInstances.Emplace(Tile.SegmentModels[s], TArray<FTransform>());

			FTransform Transform = FTransform(
				DUNGEON_SEGMENT_ROTATIONS[s],
				FVector(
					TilePoints[i].X * Scale,
					TilePoints[i].Y * Scale,
					TilePoints[i].Z * Scale + (Scale/2)),
				FVector(Scale/100 + 0.001f, Scale/100 + 0.001f, Scale/100 + 0.001f));

			FDungeonModel Model = DungeonPalette.Models.FindRef(Tile.SegmentModels[s]);
			BatchedInstances[Tile.SegmentModels[s]].Emplace(Transform);
		}

		// add all the custom tiles to the batch.
		for (int c=0; c < Tile.CustomModels.Num(); c++)
		{
			FTransform Transform = FTransform(
				FVector(
					TilePoints[i].X * Scale,
					TilePoints[i].Y * Scale,
					TilePoints[i].Z * Scale + (Scale/2)));

			if (!BatchedInstances.Contains(Tile.CustomModels[c]))
				BatchedInstances.Emplace(Tile.CustomModels[c], TArray<FTransform>());

			FDungeonModel Model = DungeonPalette.Models.FindRef(Tile.CustomModels[c]);
			BatchedInstances[Tile.CustomModels[c]].Emplace(Transform);
		}
	}

	// take all the batches and add the instances!
	TArray<FName> Templates;
	BatchedInstances.GetKeys(Templates);
	for (int i=0; i < Templates.Num(); i++)
	{
		TArray<FTransform> Transforms = BatchedInstances[Templates[i]];
		UInstancedStaticMeshComponent* ISMC = GetInstancedMeshComponent(Templates[i]);
		if (!ISMC) continue;
		ISMC->ClearInstances();
		ISMC->AddInstances(Transforms, false);

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
}

UInstancedStaticMeshComponent* ADungeon::GetInstancedMeshComponent(FName TemplateName)
{
	if (!DungeonPalette.Models.Contains(TemplateName)) return NULL;
	FDungeonModel Template = DungeonPalette.Models.FindRef(TemplateName);
	UInstancedStaticMeshComponent* ISMC = ISMCs.FindRef(TemplateName);
	if (!ISMC)
	{
		ISMC = NewObject<UInstancedStaticMeshComponent>(this);
		ISMC->NumCustomDataFloats = 4; // X,Y,Z,SEGMENT
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
