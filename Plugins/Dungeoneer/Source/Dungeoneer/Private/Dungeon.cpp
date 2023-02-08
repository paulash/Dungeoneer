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

	FDungeonSegmentTemplate WallTemplate;
	WallTemplate.Mesh = DungeonQuad;
	WallTemplate.Materials = { wall.Object };
	DungeonPalette.SegmentTemplates.Emplace("DEFAULT_WALL", WallTemplate);

	FDungeonSegmentTemplate FloorTemplate;
	FloorTemplate.Mesh = DungeonQuad;
	FloorTemplate.Materials = { floor.Object };
	DungeonPalette.SegmentTemplates.Emplace("DEFAULT_FLOOR", FloorTemplate);

	FDungeonSegmentTemplate CeilingTemplate;
	CeilingTemplate.Mesh = DungeonQuad;
	CeilingTemplate.Materials = { ceiling.Object };
	DungeonPalette.SegmentTemplates.Emplace("DEFAULT_CEILING", CeilingTemplate);

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
	NewTile.SegmentTemplates[(int)EDungeonDirection::NORTH] = "DEFAULT_WALL";
	NewTile.SegmentTemplates[(int)EDungeonDirection::EAST] = "DEFAULT_WALL";
	NewTile.SegmentTemplates[(int)EDungeonDirection::SOUTH] = "DEFAULT_WALL";
	NewTile.SegmentTemplates[(int)EDungeonDirection::WEST] = "DEFAULT_WALL";
	NewTile.SegmentTemplates[(int)EDungeonDirection::DOWN] = "DEFAULT_FLOOR";
	NewTile.SegmentTemplates[(int)EDungeonDirection::UP] = "DEFAULT_CEILING";
	
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
	Tiles[TilePoint].SegmentTemplates[(int)Segment] = Template;
#ifdef WITH_EDITOR
	GEditor->EndTransaction();
#endif
	
	RegenerateTiles();
}

void ADungeon::RegenerateTiles()
{
	TArray<UActorComponent*> CurrentISMCs;
	GetComponents(UInstancedStaticMeshComponent::StaticClass(), CurrentISMCs);
	
	for (int i=0; i < CurrentISMCs.Num(); i++)
	{
		UInstancedStaticMeshComponent* ISMC = Cast<UInstancedStaticMeshComponent>(CurrentISMCs[i]);
		if (!ISMC) continue;

		ISMC->UnregisterComponent();
		ISMC->DestroyComponent();
	}
	ISMCs.Empty();
	
	TMap<FName, TArray<FTransform>> BatchedInstances;
	
	TArray<FIntVector> TilePoints;
	Tiles.GetKeys(TilePoints);
	for (int i=0; i < TilePoints.Num(); i++)
	{
		FDungeonTile Tile = Tiles[TilePoints[i]];
		for (int s=0; s < DUNGEON_SEGMENT_COUNT; s++)
		{
			// if there is an opening in the direction, dont render that segment.
			if (Tiles.Contains(TilePoints[i] + DUNGEON_DIRECTIONS[s]))
				continue;

			if (!BatchedInstances.Contains(Tile.SegmentTemplates[s]))
				BatchedInstances.Emplace(Tile.SegmentTemplates[s], TArray<FTransform>());

			FTransform Transform = FTransform(
				DUNGEON_SEGMENT_ROTATIONS[s],
				FVector(
					TilePoints[i].X * Scale,
					TilePoints[i].Y * Scale,
					TilePoints[i].Z * Scale + (Scale/2)),
				FVector(Scale/100 + 0.001f, Scale/100 + 0.001f, Scale/100 + 0.001f));
			BatchedInstances[Tile.SegmentTemplates[s]].Emplace(Transform);
		}
	}

	TArray<FName> Templates;
	BatchedInstances.GetKeys(Templates);
	for (int i=0; i < Templates.Num(); i++)
	{
		TArray<FTransform> Transforms = BatchedInstances[Templates[i]];
		UInstancedStaticMeshComponent* ISMC = GetInstancedMeshComponent(Templates[i]);
		if (!ISMC) continue;
		ISMC->AddInstances(Transforms, false);
	}
}

UInstancedStaticMeshComponent* ADungeon::GetInstancedMeshComponent(FName TemplateName)
{
	if (!DungeonPalette.SegmentTemplates.Contains(TemplateName)) return NULL;
	FDungeonSegmentTemplate Template = DungeonPalette.SegmentTemplates.FindRef(TemplateName);
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
	}
	return ISMC;
}
