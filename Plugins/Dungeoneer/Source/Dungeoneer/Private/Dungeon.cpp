#include "Dungeon.h"
#include "UObject/ConstructorHelpers.h"

ADungeon::ADungeon()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");

	static ConstructorHelpers::FObjectFinder<UStaticMesh> quad(TEXT("/Dungeoneer/dungeon-quad.dungeon-quad"));
	static ConstructorHelpers::FObjectFinder<UMaterial> floor(TEXT("/Dungeoneer/basic-floor-material.basic-floor-material"));
	static ConstructorHelpers::FObjectFinder<UMaterial> wall(TEXT("/Dungeoneer/basic-wall-material.basic-wall-material"));
	static ConstructorHelpers::FObjectFinder<UMaterial> ceiling(TEXT("/Dungeoneer/basic-ceiling-material.basic-ceiling-material"));

	DungeonQuad = quad.Object;

	DefaultSegmentMaterials.SetNum(DUNGEON_SEGMENT_COUNT);
	DefaultSegmentMaterials[(int)EDungeonDirection::NORTH] = wall.Object;
	DefaultSegmentMaterials[(int)EDungeonDirection::EAST] = wall.Object;
	DefaultSegmentMaterials[(int)EDungeonDirection::SOUTH] = wall.Object;
	DefaultSegmentMaterials[(int)EDungeonDirection::WEST] = wall.Object;
	DefaultSegmentMaterials[(int)EDungeonDirection::DOWN] = floor.Object;
	DefaultSegmentMaterials[(int)EDungeonDirection::UP] = ceiling.Object;

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
	
	FDungeonTile NewTile = FDungeonTile();
	NewTile.SegmentMaterials = DefaultSegmentMaterials;
	Tiles.Emplace(TilePoint, NewTile);
	RegenerateTiles();
}

void ADungeon::DeleteTile(FIntVector TilePoint)
{
	if (!Tiles.Contains(TilePoint)) return;
	Tiles.Remove(TilePoint);
	RegenerateTiles();
}

void ADungeon::SetSegmentMaterial(FIntVector TilePoint, EDungeonDirection Segment, UMaterial* NewMaterial)
{
	if (Tiles.Contains(TilePoint)) return;
}

void ADungeon::RegenerateTiles()
{
	for (const TPair<UMaterial*, UInstancedStaticMeshComponent*>& pair : ISMCs)
	{
		pair.Value->UnregisterComponent();
		pair.Value->DestroyComponent();
	}
	ISMCs.Empty();
	
	TMap<UMaterial*, TArray<FTransform>> BatchedInstances;
	
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

			if (!BatchedInstances.Contains(Tile.SegmentMaterials[s]))
				BatchedInstances.Emplace(Tile.SegmentMaterials[s], TArray<FTransform>());

			FTransform Transform = FTransform(
				DUNGEON_SEGMENT_ROTATIONS[s],
				FVector(
					TilePoints[i].X * Scale,
					TilePoints[i].Y * Scale,
					TilePoints[i].Z * Scale + (Scale/2)),
				FVector(Scale/100 + 0.001f, Scale/100 + 0.001f, Scale/100 + 0.001f));
			BatchedInstances[Tile.SegmentMaterials[s]].Emplace(Transform);
		}
	}

	TArray<UMaterial*> Materials;
	BatchedInstances.GetKeys(Materials);
	for (int i=0; i < Materials.Num(); i++)
	{
		TArray<FTransform> Transforms = BatchedInstances[Materials[i]];
		UInstancedStaticMeshComponent* ISMC = GetInstancedMeshComponent(Materials[i]);
		ISMC->AddInstances(Transforms, false);
	}
}

UInstancedStaticMeshComponent* ADungeon::GetInstancedMeshComponent(UMaterial* Material)
{
	UInstancedStaticMeshComponent* ISMC = ISMCs.FindRef(Material);
	if (!ISMC)
	{
		ISMC = NewObject<UInstancedStaticMeshComponent>(this);
		ISMC->NumCustomDataFloats = 4; // X,Y,Z,SEGMENT
		AddInstanceComponent(ISMC);
		ISMC->RegisterComponent();
		ISMC->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		ISMC->SetStaticMesh(DungeonQuad);
		ISMC->SetMaterial(0, Material);
		ISMCs.Emplace(Material, ISMC);
	}
	return ISMC;
}
