#include "Dungeon.h"
#include "UObject/ConstructorHelpers.h"

ADungeon::ADungeon()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");

	static ConstructorHelpers::FObjectFinder<UStaticMesh> quad(TEXT("/Dungeoneer/dungeon-quad.dungeon-quad"));
	static ConstructorHelpers::FObjectFinder<UMaterial> floor(TEXT("/Dungeoneer/basic-floor-material.basic-floor-material"));
	static ConstructorHelpers::FObjectFinder<UMaterial> wall(TEXT("/Dungeoneer/basic-wall-material.basic-wall-material"));
	static ConstructorHelpers::FObjectFinder<UMaterial> ceiling(TEXT("/Dungeoneer/basic-ceiling-material.basic-ceiling-material"));
	
	DefaultFloor = FMeshMaterialPair(floor.Object, quad.Object);
	DefaultWall = FMeshMaterialPair(wall.Object, quad.Object);
	DefaultCeiling = FMeshMaterialPair(ceiling.Object, quad.Object);

	static ConstructorHelpers::FObjectFinder<UMaterial> selection(TEXT("/Dungeoneer/selection-border-material.selection-border-material"));
	SelectionMaterial = selection.Object;

	static ConstructorHelpers::FObjectFinder<UMaterial> plusIcon(TEXT("/Dungeoneer/plus-icon-material.plus-icon-material"));
	PlusIconMaterial = plusIcon.Object;
}

void ADungeon::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	RegenerateTiles();
}

void ADungeon::RegenerateTiles()
{
	TSet<UInstancedStaticMeshComponent*> UnusedISMCs;
	for (const TPair<FMeshMaterialPair, UInstancedStaticMeshComponent*>& pair : InstancedStaticMeshComponents) {
		UnusedISMCs.Emplace(pair.Value);
	}

	// if there are no floors, generate one with just one tile at 0,0
	// to get things started.
	if (Levels.Num() == 0)
		CreateLevel();

	TMap<UInstancedStaticMeshComponent*, TArray<FTransform>> NewTransforms;
	TArray<FIntPoint> TilePoints;
	for (int l=0; l < Levels.Num(); l++)
	{
		Levels[l].Tiles.GetKeys(TilePoints);
		for (int t=0; t < TilePoints.Num(); t++)
		{
			FDungeonTile tile;
			if (GetTile(l, TilePoints[t], tile))
			{
				for (int s=0; s < tile.Segments.Num(); s++)
				{
					FDungeonTile neighborTile;
					if (s <= WALL_INDEX &&
						!HasWallOverride(l, TilePoints[t], DUNGEON_DIRECTIONS[s]) &&
						GetTile(l, TilePoints[t] + FIntPoint(DUNGEON_DIRECTIONS[s].X, DUNGEON_DIRECTIONS[s].Y), neighborTile))
					{
						continue;
					}
					
					UStaticMesh* mesh = NULL;
					UMaterial* material = NULL;

					// prioritize the mesh/materials from segment/floor/default sets.
					FMeshMaterialPair SegmentPair = FMeshMaterialPair(tile.Segments[s].Material, tile.Segments[s].Mesh);
					FMeshMaterialPair FloorPair;
					FMeshMaterialPair DefaultPair;
					
					if (s == FLOOR_INDEX)
					{
						FloorPair = Levels[l].Floor;
						DefaultPair = DefaultFloor;
					}
					else if (s == CEILING_INDEX)
					{
						FloorPair = Levels[l].Ceiling;
						DefaultPair = DefaultCeiling;
					}
					else
					{
						FloorPair = Levels[l].Wall;
						DefaultPair = DefaultWall;
					}

					if (SegmentPair.Mesh)
						mesh = SegmentPair.Mesh;
					else if (FloorPair.Mesh)
						mesh = FloorPair.Mesh;
					else
						mesh = DefaultPair.Mesh;

					if (SegmentPair.Material)
						material = SegmentPair.Material;
					else if (FloorPair.Material)
						material = FloorPair.Material;
					else
						material = DefaultPair.Material;
					
					UInstancedStaticMeshComponent* ISMC = GetInstancedStaticMeshComponent(material, mesh);
					if (!NewTransforms.Contains(ISMC))
						NewTransforms.Emplace(ISMC, TArray<FTransform>());

					NewTransforms[ISMC].Emplace(
						FTransform(
						DUNGEON_SEGMENT_ROTATIONS[s],
						FVector(
							TilePoints[t].X * DungeonScale,
							TilePoints[t].Y * DungeonScale,
							l * DungeonLevelZ)
					));
					UnusedISMCs.Remove(ISMC); // was used, so remove it from the unused.
				}
			}
		}
	}
	
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

UInstancedStaticMeshComponent* ADungeon::GetInstancedStaticMeshComponent(UMaterial* Material, UStaticMesh* Mesh)
{
	UInstancedStaticMeshComponent* ISMC = InstancedStaticMeshComponents.FindRef(FMeshMaterialPair(Material, Mesh));
	if (!ISMC)
	{
		ISMC = NewObject<UInstancedStaticMeshComponent>(this);
		AddInstanceComponent(ISMC);
		ISMC->RegisterComponent();
		ISMC->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		ISMC->SetRelativeLocation(FVector(0,0, -(DungeonLevelZ/2)));
		ISMC->SetAbsolute(false, true, true);
		ISMC->SetStaticMesh(Mesh);
		ISMC->SetMaterial(0, Material);

		InstancedStaticMeshComponents.Emplace(FMeshMaterialPair(Material, Mesh), ISMC);
	}
	return ISMC;
}
