#include "Dungeon.h"
#include "UObject/ConstructorHelpers.h"

ADungeon::ADungeon()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");

	static ConstructorHelpers::FObjectFinder<UStaticMesh> quad(TEXT("/Dungeoneer/dungeon-quad.dungeon-quad"));
	static ConstructorHelpers::FObjectFinder<UMaterial> floor(TEXT("/Dungeoneer/basic-floor-material.basic-floor-material"));
	static ConstructorHelpers::FObjectFinder<UMaterial> wall(TEXT("/Dungeoneer/basic-wall-material.basic-wall-material"));
	static ConstructorHelpers::FObjectFinder<UMaterial> ceiling(TEXT("/Dungeoneer/basic-ceiling-material.basic-ceiling-material"));

	TArray<UMaterial*> DefaultFloorMaterials = {floor.Object};
	DefaultFloor = FDungeonModel(DefaultFloorMaterials, quad.Object);

	TArray<UMaterial*> DefaultWallMaterials = {wall.Object};
	DefaultWall = FDungeonModel(DefaultWallMaterials, quad.Object);

	TArray<UMaterial*> DefaultCeilingMaterials = {ceiling.Object};
	DefaultCeiling = FDungeonModel(DefaultCeilingMaterials, quad.Object);

	DefaultTileTemplate = FDungeonTileTemplate();
	DefaultTileTemplate.NorthWall = DefaultWall;
	DefaultTileTemplate.EastWall = DefaultWall;
	DefaultTileTemplate.SouthWall = DefaultWall;
	DefaultTileTemplate.WestWall = DefaultWall;
	
	DefaultTileTemplate.Floor = DefaultFloor;
	DefaultTileTemplate.Ceiling = DefaultCeiling;
}

void ADungeon::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (Tiles.Num() == 0)
		CreateTile(FIntVector::ZeroValue, DefaultTileTemplate);
	else
		RegenerateTiles();
}

void ADungeon::RegenerateTiles()
{
	TSet<UInstancedStaticMeshComponent*> UnusedISMCs;
	for (const TPair<FDungeonModel, UInstancedStaticMeshComponent*>& pair : ISMCs)
		UnusedISMCs.Emplace(pair.Value);

	TMap<UInstancedStaticMeshComponent*, TArray<FTransform>> NewTransforms;
	TArray<FIntVector> TilePoints;
	Tiles.GetKeys(TilePoints);
	for (int t=0; t < TilePoints.Num(); t++)
	{
		FDungeonTile Tile;
		if (GetTile(TilePoints[t], Tile))
		{
			for (int s=0; s < Tile.Segments.Num(); s++)
			{
				// if there is not an override, and there is a tile, in the direction
				// of this segment, we dont render this segment.
				if (!HasSegmentOverride(l, TilePoints[t], DUNGEON_DIRECTIONS[s]) &&
					HasTile(TilePoints[t] + DUNGEON_DIRECTIONS[s]))
				{
					continue;
				}

				UInstancedStaticMeshComponent* ISMC = GetInstancedStaticMeshComponent(Tile.Segments[s].Model);
				if (!NewTransforms.Contains(ISMC))
					NewTransforms.Emplace(ISMC, TArray<FTransform>());

				NewTransforms[ISMC].Emplace(
					FTransform(
					DUNGEON_SEGMENT_ROTATIONS[s],
					FVector(
						TilePoints[t].X * Scale,
						TilePoints[t].Y * Scale,
						TilePoints[t].Z * Scale)
				));
				UnusedISMCs.Remove(ISMC); // was used, so remove it from the unused.
			}
		}
	}

	// clear and apply the new instances.
	for (const TPair<UInstancedStaticMeshComponent*, TArray<FTransform>>& pair : NewTransforms)
	{
		pair.Key->ClearInstances();
		pair.Key->AddInstances(pair.Value, false);
	}

	TArray<UInstancedStaticMeshComponent*> FinalUnusedISMCs = UnusedISMCs.Array();
	for (int i=0; i < FinalUnusedISMCs.Num(); i++)
	{
		FinalUnusedISMCs[i]->UnregisterComponent();
		FinalUnusedISMCs[i]->DestroyComponent();
	}
}

bool ADungeon::CreateTile(FIntVector TilePoint, FDungeonTileTemplate& Template)
{
	if (Tiles.Contains(TilePoint))
		return false;
	
	FDungeonTile NewTile = FDungeonTile();

	NewTile.Segments[(int)EDungeonTileSegment::NORTH_WALL].Model = Template.NorthWall;
	NewTile.Segments[(int)EDungeonTileSegment::EAST_WALL].Model = Template.EastWall;
	NewTile.Segments[(int)EDungeonTileSegment::SOUTH_WALL].Model = Template.SouthWall;
	NewTile.Segments[(int)EDungeonTileSegment::WEST_WALL].Model = Template.WestWall;
	
	NewTile.Segments[(int)EDungeonTileSegment::FLOOR].Model = Template.Floor;
	NewTile.Segments[(int)EDungeonTileSegment::CEILING].Model = Template.Ceiling;

	Tiles.Emplace(TilePoint, NewTile);
	RegenerateTiles();
	return true;
}

bool ADungeon::HasTile(FIntVector TilePoint)
{
	return Tiles.Contains(TilePoint);
}

bool ADungeon::GetTile(FIntVector TilePoint, FDungeonTile& Tile)
{
	if (!Tiles.Contains(TilePoint))
		return false;
	
	Tile = Tiles.FindRef(TilePoint);
	return true;
}

bool ADungeon::DeleteTile(FIntVector TilePoint)
{
	if (Tiles.Contains(TilePoint))
		return false;

	Tiles.Remove(TilePoint);
	RegenerateTiles();
	return true;
}

bool ADungeon::CreateSegmentOverride(FIntVector TilePoint, EDungeonTileSegment Segment)
{
	NormalizeSegmentOverride(TilePoint, Segment);
	FDungeonSegmentOverride SegmentOverride = FDungeonSegmentOverride(TilePoint, Segment);
	if (SegmentOverrides.Contains(SegmentOverride))
		return false;
	
	SegmentOverrides.Emplace(SegmentOverride);
	RegenerateTiles();
	return true;
}

bool ADungeon::HasSegmentOverride(FIntVector TilePoint, EDungeonTileSegment Segment)
{
	NormalizeSegmentOverride(TilePoint, Segment);
	FDungeonSegmentOverride SegmentOverride = FDungeonSegmentOverride(TilePoint, Segment);
	return SegmentOverrides.Contains(SegmentOverride);
}

bool ADungeon::DeleteSegmentOverride(FIntVector TilePoint, EDungeonTileSegment Segment)
{
	NormalizeSegmentOverride(TilePoint, Segment);
	FDungeonSegmentOverride SegmentOverride = FDungeonSegmentOverride(TilePoint, Segment);
	if (!SegmentOverrides.Contains(SegmentOverride))
		return false;

	SegmentOverrides.Remove(SegmentOverride);
	RegenerateTiles();
	return true;
}

UInstancedStaticMeshComponent* ADungeon::GetInstancedStaticMeshComponent(FDungeonModel& DungeonModel)
{
	UInstancedStaticMeshComponent* ISMC = ISMCs.FindRef(DungeonModel);
	if (!ISMC)
	{
		ISMC = NewObject<UInstancedStaticMeshComponent>(this);
		AddInstanceComponent(ISMC);
		ISMC->RegisterComponent();
		ISMC->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		ISMC->SetRelativeLocation(FVector(0,0, -(Scale/2)));
		ISMC->SetAbsolute(false, true, true);
		ISMC->SetStaticMesh(DungeonModel.Mesh);

		for (int i=0; i < DungeonModel.Materials.Num(); i++)
			ISMC->SetMaterial(i, DungeonModel.Materials[i]);	

		ISMCs.Emplace(DungeonModel, ISMC);
	}
	return ISMC;
}
