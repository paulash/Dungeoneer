#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Misc/Crc.h"
#include "Components/ChildActorComponent.h"
#include "GameplayTagContainer.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Dungeon.generated.h"

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
	UMaterial* Material = NULL;

	UPROPERTY(EditAnywhere)
	UStaticMesh* Mesh = NULL;

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
struct FDungeonWallHash
{
	GENERATED_BODY();

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FIntPoint Point = FIntPoint(0, 0);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FIntPoint Direction = FIntPoint(0, 0);

	FDungeonWallHash()
	{
		Point = FIntPoint(0, 0);
		Direction = FIntPoint(0, 0);
	}

	FDungeonWallHash(FIntPoint _Point, FIntPoint _Direction)
	{
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
		return Point == Other.Point && Direction == Other.Direction;
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
struct FDungeonTileSegment
{
	GENERATED_BODY();

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTagContainer Tags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UMaterial* Material = NULL;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMesh* Mesh = NULL;
};

USTRUCT(BlueprintType)
struct FDungeonTile
{
	GENERATED_BODY();

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTagContainer Tags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FDungeonTileSegment> Segments;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool Blocked = false;

	FDungeonTile()
	{
		for (int i=0; i < DUNGEON_TILE_SEGMENT_COUNT; i++)
			Segments.Emplace(FDungeonTileSegment());
	}
	
};

USTRUCT(BlueprintType)
struct FDungeonLevel
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

	UPROPERTY(EditAnywhere)
	TSet<FDungeonWallHash> WallOverrides;
	
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
	float DungeonLevelZ = -100.0f;

	UPROPERTY(EditAnywhere)
	TArray<FDungeonLevel> Levels;

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
	bool GetFloor(int LevelIndex, FDungeonLevel& OutFloor)
	{
		if (!Levels.IsValidIndex(LevelIndex)) return false;
		OutFloor = Levels[LevelIndex];
		return true;
	}

	UFUNCTION(BlueprintCallable)
	int CreateLevel()
	{
		FDungeonTile tile;
		FDungeonLevel floor = FDungeonLevel();
		int LevelIndex = Levels.Emplace(floor);

		// always create a 0,0 tile for a new room.
		if (CreateTile(LevelIndex, FIntPoint(0,0), tile))
		{
			RegenerateTiles();
			return true;
		}
		Levels.RemoveAt(LevelIndex);
		return false;
	}

	UFUNCTION(BlueprintCallable)
	bool DeleteLevel(int LevelIndex)
	{
		if (Levels.Num() <= 1) return false; // can't delete the last level
		if (Levels.IsValidIndex(LevelIndex))
		{
			Levels.RemoveAt(LevelIndex);
			RegenerateTiles();
			return true;
		}
		return false;
	}

	UFUNCTION(BlueprintCallable)
	bool GetTile(int LevelIndex, FIntPoint TilePoint, FDungeonTile& OutTile)
	{
		FDungeonLevel OutFloor;
		if (GetFloor(LevelIndex, OutFloor))
			return OutFloor.GetTile(TilePoint, OutTile);
		return false;
	}

	UFUNCTION(BlueprintCallable)
	bool HasTile(int LevelIndex, FIntPoint TilePoint)
	{
		if (!Levels.IsValidIndex(LevelIndex)) return false;
		return Levels[LevelIndex].Tiles.Contains(TilePoint);
	}
	
	UFUNCTION(BlueprintCallable)
	bool CreateTile(int LevelIndex, FIntPoint Point, FDungeonTile& OutTile)
	{
		// no such floor.
		if (!Levels.IsValidIndex(LevelIndex))
			return false;

		// tile already exists.
		if (GetTile(LevelIndex, Point, OutTile))
			return false;
		
		OutTile = FDungeonTile();
		
		Levels[LevelIndex].Tiles.Emplace(Point, OutTile);
		RegenerateTiles();
		return true;
	}

	UFUNCTION(BlueprintCallable)
	bool DeleteTile(int LevelIndex, FIntPoint TilePoint)
	{
		if (!Levels.IsValidIndex(LevelIndex)) return false;
		if (!Levels[LevelIndex].Tiles.Contains(TilePoint)) return false;

		// delete the tiles.
		// delete any wall overrides associated.

		Levels[LevelIndex].Tiles.Remove(TilePoint);
		for (int i=0; i < DUNGEON_DIRECTIONS.Num(); i++)
			DeleteWallOverride(LevelIndex, TilePoint, DUNGEON_DIRECTIONS[i]);

		RegenerateTiles();
		return true;
	}

	UFUNCTION(BlueprintCallable)
	bool HasWallOverride(int LevelIndex, FIntPoint TilePoint, FIntPoint Direction)
	{
		if (!Levels.IsValidIndex(LevelIndex))
			return false;
		
		if (Direction == SOUTH_POINT)
		{
			TilePoint += Direction;
			Direction = NORTH_POINT;
		}
		if (Direction == WEST_POINT)
		{
			TilePoint += Direction;
			Direction = EAST_POINT;
		}
		return Levels[LevelIndex].WallOverrides.Contains(FDungeonWallHash(TilePoint, Direction));
	}
	
	UFUNCTION(BlueprintCallable)
	bool CreateWallOverride(int LevelIndex, FIntPoint TilePoint, FIntPoint Direction)
	{
		if (!Levels.IsValidIndex(LevelIndex))
			return false;
		
		if (HasWallOverride(LevelIndex, TilePoint, Direction))
			return false;
		
		if (Direction == SOUTH_POINT)
		{
			TilePoint += Direction;
			Direction = NORTH_POINT;
		}
		if (Direction == WEST_POINT)
		{
			TilePoint += Direction;
			Direction = EAST_POINT;
		}
		// flip south and west, to north and east, to ensure tile points and directions
		// are compacted to the shared space between them.
		Levels[LevelIndex].WallOverrides.Emplace(FDungeonWallHash(TilePoint, Direction));
		RegenerateTiles();
		return true;
	}

	UFUNCTION(BlueprintCallable)
	bool DeleteWallOverride(int LevelIndex, FIntPoint TilePoint, FIntPoint Direction)
	{
		if (!Levels.IsValidIndex(LevelIndex))
			return false;
		
		if (Direction == SOUTH_POINT)
		{
			TilePoint += Direction;
			Direction = NORTH_POINT;
		}
		if (Direction == WEST_POINT)
		{
			TilePoint += Direction;
			Direction = EAST_POINT;
		}
		Levels[LevelIndex].WallOverrides.Remove(FDungeonWallHash(TilePoint, Direction));
		return true;
	}
	
	UFUNCTION(BlueprintCallable)
	bool IsBlocked(int LevelIndex, FIntPoint TilePoint, FIntPoint Direction)
	{
		if (!Levels.IsValidIndex(LevelIndex))
			return true;

		FDungeonTile tile;
		if (GetTile(LevelIndex, TilePoint + Direction, tile) && tile.Blocked)
			return true;

		if (HasWallOverride(LevelIndex, TilePoint, Direction))
			return true;

		return false;
	}

	// add tag helpers.
	// need 'CheckWallTag' checks if a wall segment in a specific tile/direction has a flag.
	// need 'AddWallTag'
	// need 'RemoveWallTag'
	// need 'AddTileTag'
	// need 'RemoveTileTag'
	// need 'AddLevelTag'
	// need 'RemoveLevelTag'

	UPROPERTY()
	UMaterial* SelectionMaterial;
	
	UPROPERTY()
	UMaterialInstanceDynamic* TileSelectedMaterial;

	UPROPERTY()
	UMaterialInstanceDynamic* TileUnselectedMaterial;
	
private:
	
	UPROPERTY()
	TMap<FMeshMaterialPair, UInstancedStaticMeshComponent*> InstancedStaticMeshComponents;

	UInstancedStaticMeshComponent* GetInstancedStaticMeshComponent(UMaterial* Material, UStaticMesh* Mesh);

};