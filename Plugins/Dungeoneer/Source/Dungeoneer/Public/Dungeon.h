#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Misc/Crc.h"
#include "Components/ChildActorComponent.h"
#include "GameplayTagContainer.h"
#include "Dungeon.generated.h"

UENUM(BlueprintType)
enum class EDungeonTileSegment : uint8
{
	NORTH,	// wall
	SOUTH,	// wall
	EAST,	// wall
	WEST,	// wall
	DOWN,	// floor
	UP		// ceiling
};

#define DUNGEON_TILE_SEGMENT_COUNT 6

UENUM(BlueprintType)
enum class EDungeonDirection : uint8
{
	NORTH,
	SOUTH,
	EAST,
	WEST
};

#define NORTH_POINT FIntPoint(1, 0)
#define EAST_POINT FIntPoint(0, 1)
#define SOUTH_POINT FIntPoint(-1, 0)
#define WEST_POINT FIntPoint(0, -1)

#define WALL_INDEX 3
#define FLOOR_INDEX 4
#define CEILING_INDEX 5

const static TArray<FIntPoint> DUNGEON_DIRECTIONS = {
	NORTH_POINT,
	EAST_POINT,
	SOUTH_POINT,
	WEST_POINT
};

const static TArray<FRotator> DUNGEON_SEGMENT_ROTATIONS = {
	FRotator(180, 90, 90),	// NORTH
	FRotator(0, 0, -90),	// EAST
	FRotator(0, 90, -90),	// SOUTH
	FRotator(180, 0, 90),	// WEST
	FRotator(0, 0, 00),		// DOWN
	FRotator(180, 0, 0),	// UP
};

USTRUCT(BlueprintType)
struct FMeshMaterialPair
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	UMaterial* Material;

	UPROPERTY(EditAnywhere)
	UStaticMesh* Mesh;

	FMeshMaterialPair()
		: Material(NULL), Mesh(NULL)
	{}

	FMeshMaterialPair(UMaterial* material, UStaticMesh* mesh)
		: Material(material), Mesh(mesh)
	{}

	FMeshMaterialPair(const FMeshMaterialPair& Other)
		: FMeshMaterialPair(Other.Material, Other.Mesh)
	{}

	bool operator==(const FMeshMaterialPair& Other) const
	{
		return Equals(Other);
	}

	bool operator!=(const FMeshMaterialPair& Other) const
	{
		return !Equals(Other);
	}

	bool Equals(const FMeshMaterialPair& Other) const
	{
		return Material == Other.Material && Mesh == Other.Mesh;
	}
};

#if UE_BUILD_DEBUG // debuggable and slower.
uint32 GetTypeHash(const FMeshMaterialPair& Thing)
{
	uint32 Hash = FCrc::MemCrc32(&Thing, sizeof(FMeshMaterialPair));
	return Hash;
}
#else // optimize by inlining in shipping and development builds
FORCEINLINE uint32 GetTypeHash(const FMeshMaterialPair& Thing)
{
	uint32 Hash = FCrc::MemCrc32(&Thing, sizeof(FMeshMaterialPair));
	return Hash;
}
#endif

USTRUCT(BlueprintType)
struct FDungeonTileSegment
{
	GENERATED_BODY();

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EDungeonTileSegment Segment;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTagContainer Tags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UMaterial* Material;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMesh* Mesh;
};

USTRUCT(BlueprintType)
struct FDungeonWallHash
{
	GENERATED_BODY();

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int FloorIndex;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FIntPoint Point;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FIntPoint Direction;

	FDungeonWallHash()
	{
		FloorIndex = -1;
		Point = FIntPoint();
		Direction = FIntPoint();
	}

	FDungeonWallHash(int _FloorIndex, FIntPoint _Point, FIntPoint _Direction)
	{
		FloorIndex = _FloorIndex;
		Point = _Point;
		Direction = _Direction;
	}

	bool operator==(const FDungeonWallHash& Other) const
	{
		return Equals(Other);
	}

	bool operator!=(const FDungeonWallHash& Other) const
	{
		return !Equals(Other);
	}

	bool Equals(const FDungeonWallHash& Other) const
	{
		return FloorIndex == Other.FloorIndex && Point == Other.Point && Direction == Other.Direction;
	}
};
#if UE_BUILD_DEBUG // debuggable and slower.
uint32 GetTypeHash(const FDungeonWallHash& Thing)
{
	uint32 Hash = FCrc::MemCrc32(&Thing, sizeof(FDungeonWallHash));
	return Hash;
}
#else // optimize by inlining in shipping and development builds
FORCEINLINE uint32 GetTypeHash(const FDungeonWallHash& Thing)
{
	uint32 Hash = FCrc::MemCrc32(&Thing, sizeof(FDungeonWallHash));
	return Hash;
}
#endif

USTRUCT(BlueprintType)
struct FDungeonTile
{
	GENERATED_BODY();

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTagContainer Tags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FDungeonTileSegment> Segments;

	UPROPERTY()
	FIntPoint Point;
};

USTRUCT(BlueprintType)
struct FDungeonFloor
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere)
	FGameplayTagContainer Tags;
	
	UPROPERTY(EditAnywhere)
	FMeshMaterialPair Floor;

	UPROPERTY(EditAnywhere)
	FMeshMaterialPair Wall;

	UPROPERTY(EditAnywhere)
	FMeshMaterialPair Ceiling;

	UPROPERTY(EditAnywhere)
	TMap<FIntPoint, FDungeonTile> Tiles; 
	
	bool GetTile(FIntPoint Point, FDungeonTile& OutTile)
	{
		if (!Tiles.Contains(Point)) return false;
		OutTile = Tiles[Point];
		return true;
	}
};

UCLASS(NotPlaceable, BlueprintType, NotBlueprintable)
class ADungeon final : public AActor
{
	GENERATED_BODY()

public:

	// controls the scale of the meshes and distance between them.
	UPROPERTY(EditAnywhere)
	float DungeonScale = 100.0f;

	// distance between each floor along the Z axis.
	UPROPERTY(EditAnywhere)
	float DungeonFloorZ = -100.0f;

	UPROPERTY(EditAnywhere)
	TArray<FDungeonFloor> Floors;

	UPROPERTY(EditAnywhere)
	TSet<FDungeonWallHash> Walls;

	UPROPERTY(EditAnywhere)
	FMeshMaterialPair DefaultFloor;

	UPROPERTY(EditAnywhere)
	FMeshMaterialPair DefaultWall;

	UPROPERTY(EditAnywhere)
	FMeshMaterialPair DefaultCeiling;

	ADungeon();

	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION(BlueprintCallable)
	void RegenerateTiles();

	UFUNCTION(BlueprintCallable)
	bool GetFloor(int FloorIndex, FDungeonFloor& OutFloor)
	{
		if (!Floors.IsValidIndex(FloorIndex)) return false;
		OutFloor = Floors[FloorIndex];
		return true;
	}

	UFUNCTION(BlueprintCallable)
	bool GetTile(int FloorIndex, FIntPoint Point, FDungeonTile& OutTile)
	{
		FDungeonFloor OutFloor;
		if (GetFloor(FloorIndex, OutFloor))
			return OutFloor.GetTile(Point, OutTile);
		return false;
	}

	UFUNCTION(BlueprintCallable)
	int CreateFloor()
	{
		FDungeonTile tile;
		FDungeonFloor floor = FDungeonFloor();
		int floorIndex = Floors.Emplace(floor);

		// always create a 0,0 tile for a new room.
		return CreateTile(floorIndex, FIntPoint(0,0), tile); 
	}

	UFUNCTION(BlueprintCallable)
	bool CreateTile(int FloorIndex, FIntPoint Point, FDungeonTile& OutTile)
	{
		// no such floor.
		if (!Floors.IsValidIndex(FloorIndex))
			return false;

		// tile already exists.
		if (GetTile(FloorIndex, Point, OutTile))
			return false;

		// generate segments for the tile.
		OutTile = FDungeonTile();
		for (int i=0; i < DUNGEON_TILE_SEGMENT_COUNT; i++)
			OutTile.Segments.Emplace(FDungeonTileSegment());
		
		Floors[FloorIndex].Tiles.Emplace(Point, OutTile);
		return true;
	}
	
	UFUNCTION(BlueprintCallable)
	bool CreateWall(int FloorIndex, FIntPoint Tile, FIntPoint Direction)
	{
		if (GetWall(FloorIndex, Tile, Direction))
			return false;
		
		if (Direction == SOUTH_POINT)
		{
			Tile += Direction;
			Direction = NORTH_POINT;
		}
		if (Direction == WEST_POINT)
		{
			Tile += Direction;
			Direction = EAST_POINT;
		}
		// flip south and west, to north and east, to ensure tile points and directions
		// are compacted to the shared space between them.
		Walls.Emplace(FDungeonWallHash(FloorIndex, Tile, Direction));
		return true;
	}
	
	UFUNCTION(BlueprintCallable)
	bool GetWall(int FloorIndex, FIntPoint Tile, FIntPoint Direction)
	{
		if (Direction == SOUTH_POINT)
		{
			Tile += Direction;
			Direction = NORTH_POINT;
		}
		if (Direction == WEST_POINT)
		{
			Tile += Direction;
			Direction = EAST_POINT;
		}
		return Walls.Contains(FDungeonWallHash(FloorIndex, Tile, Direction));
	}
	
private:

	UPROPERTY()
	TMap<FMeshMaterialPair, UInstancedStaticMeshComponent*> InstancedStaticMeshComponents;

	UInstancedStaticMeshComponent* GetInstancedStaticMeshComponent(UMaterial* Material, UStaticMesh* Mesh);

};