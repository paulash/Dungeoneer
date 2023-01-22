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
}

void ADungeon::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	RegenerateTiles();
}

void ADungeon::RegenerateTiles()
{
	for (const TPair<FMeshMaterialPair, UInstancedStaticMeshComponent*>& pair : InstancedStaticMeshComponents) {
		pair.Value->UnregisterComponent();
		pair.Value->DestroyComponent();
	}
	InstancedStaticMeshComponents.Empty();

	// if there are no floors, generate one with just one tile at 0,0
	// to get things started.
	if (Floors.Num() == 0)
	{
		CreateFloor();
	}

	TArray<FIntPoint> TilePoints;
	for (int f=0; f < Floors.Num(); f++)
	{
		Floors[f].Tiles.GetKeys(TilePoints);
		for (int t=0; t < TilePoints.Num(); t++)
		{
			FDungeonFloor floor;
			FDungeonTile tile;
			if (GetTile(f, TilePoints[t], floor, tile))
			{
				for (int s=0; s < tile.Segments.Num(); s++)
				{
					FMeshMaterialPair Pair;
					if (s <= WALL_INDEX)
						Pair = DefaultWall;
					else if (s == FLOOR_INDEX)
						Pair = DefaultFloor;
					else if (s == CEILING_INDEX)
						Pair = DefaultCeiling;
					
					UStaticMesh* mesh = tile.Segments[s].Mesh ? tile.Segments[s].Mesh : Pair.Mesh;
					UMaterial* material = tile.Segments[s].Material ? tile.Segments[s].Material : Pair.Material;
					
					UInstancedStaticMeshComponent* ISMC = GetInstancedStaticMeshComponent(material, mesh);
					if (!ISMC)
						continue; // TODO: report an error.
					ISMC->AddInstance(FTransform(
						DUNGEON_SEGMENT_ROTATIONS[s],
						FVector(
							TilePoints[t].X * DungeonScale,
							TilePoints[t].Y * DungeonScale,
							f * DungeonFloorZ)
						)
					);
				}
			}
		}
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
		ISMC->SetStaticMesh(Mesh);
		ISMC->SetMaterial(0, Material);

		InstancedStaticMeshComponents.Emplace(FMeshMaterialPair(Material, Mesh), ISMC);
	}
	return ISMC;
}
