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
	WEST,
	DOWN,
	UP
};

UENUM(BlueprintType)
enum class EDungeonTileSegment : uint8
{
	NORTH_WALL,
	SOUTH_WALL,
	EAST_WALL,
	WEST_WALL,
	FLOOR,
	CEILING
};

#define NORTH_POINT FIntVector(1, 0, 0)
#define EAST_POINT FIntVector(0, 1, 0)
#define SOUTH_POINT FIntVector(-1, 0, 0)
#define WEST_POINT FIntVector(0, -1, 0)
#define FLOOR_POINT FIntVector(0, 0, -1)
#define CEILING_POINT FIntVector(0,0, 1)

#define WALL_INDEX 3
#define FLOOR_INDEX 4
#define CEILING_INDEX 5

const static TArray<FIntVector> DUNGEON_DIRECTIONS = {
	NORTH_POINT,
	EAST_POINT,
	SOUTH_POINT,
	WEST_POINT,
	FLOOR_POINT,
	CEILING_POINT
};

const static TArray<FRotator> DUNGEON_SEGMENT_ROTATIONS = {
	FRotator(180, 90, 90),	// NORTH
	FRotator(0, 0, -90),		// EAST
	FRotator(0, 90, -90),	// SOUTH
	FRotator(180, 0, 90),	// WEST
	FRotator(0, 0, 0),		// FLOOR
	FRotator(180, 0, 0),		// CEILING
};

USTRUCT(BlueprintType)
struct FDungeonModel
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	TArray<UMaterial*> Materials;

	UPROPERTY(EditAnywhere)
	UStaticMesh* Mesh = NULL;

	FDungeonModel()
		:  Mesh(NULL)
	{}

	FDungeonModel(TArray<UMaterial*> materials, UStaticMesh* mesh)
		: Materials(materials), Mesh(mesh)
	{}

	FDungeonModel(const FDungeonModel& Other)
		: FDungeonModel(Other.Materials, Other.Mesh)
	{}

	bool operator==(const FDungeonModel& Other) const
	{
		return Equals(Other);
	}

	bool operator!=(const FDungeonModel& Other) const
	{
		return !Equals(Other);
	}

	bool Equals(const FDungeonModel& Other) const
	{
		if (Materials.Num() != Other.Materials.Num()) return false;
		for (int i=0; i < Materials.Num(); i++)
		{
			if (Materials[i] != Other.Materials[i])
				return false;
		}
		
		return Mesh == Other.Mesh;
	}
};
#if UE_BUILD_DEBUG // debuggable and slower.
uint32 GetTypeHash(const FDungeonModel& Thing)
{
	uint32 Hash = FCrc::MemCrc32(&Thing, sizeof(FDungeonModel));
	return Hash;
}
#else // optimize by inlining in shipping and development builds
FORCEINLINE uint32 GetTypeHash(const FDungeonModel& Thing)
{
	uint32 Hash = FCrc::MemCrc32(&Thing, sizeof(FDungeonModel));
	return Hash;
}
#endif

USTRUCT(BlueprintType)
struct FDungeonSegmentOverride
{
	GENERATED_BODY();

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FIntVector TilePoint = FIntVector::ZeroValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EDungeonTileSegment Segment = EDungeonTileSegment::FLOOR;

	FDungeonSegmentOverride()
	{
		TilePoint = FIntVector::ZeroValue;
		Segment = EDungeonTileSegment::FLOOR;
	}

	FDungeonSegmentOverride(FIntVector _TilePoint, EDungeonTileSegment _Segment)
	{
		TilePoint = _TilePoint;
		Segment = _Segment;
	}

	bool operator==(const FDungeonSegmentOverride& Other) const
	{
		return Equals(Other);
	}

	bool operator!=(const FDungeonSegmentOverride& Other) const
	{
		return !Equals(Other);
	}

	bool Equals(const FDungeonSegmentOverride& Other) const
	{
		return TilePoint == Other.TilePoint && Segment == Other.Segment;
	}
};
#if UE_BUILD_DEBUG // debuggable and slower.
uint32 GetTypeHash(const FDungeonSegmentOverride& Thing)
{
	uint32 Hash = FCrc::MemCrc32(&Thing, sizeof(FDungeonSegmentOverride));
	return Hash;
}
#else // optimize by inlining in shipping and development builds
FORCEINLINE uint32 GetTypeHash(const FDungeonSegmentOverride& Thing)
{
	uint32 Hash = FCrc::MemCrc32(&Thing, sizeof(FDungeonSegmentOverride));
	return Hash;
}
#endif

USTRUCT(BlueprintType)
struct FDungeonTileSegmentHash
{
	GENERATED_BODY()

public:


	UPROPERTY()
	FIntVector TilePoint;

	UPROPERTY()
	EDungeonTileSegment Segment;

	FDungeonTileSegmentHash()
	{
		TilePoint = FIntVector::ZeroValue;
		Segment = EDungeonTileSegment::NORTH_WALL;
	}

	FDungeonTileSegmentHash(FIntVector _TilePoint, EDungeonTileSegment _Segment)
	{
		TilePoint = _TilePoint;
		Segment = _Segment;
	}

	bool operator==(const FDungeonTileSegmentHash& Other) const
	{
		return Equals(Other);
	}

	bool operator!=(const FDungeonTileSegmentHash& Other) const
	{
		return !Equals(Other);
	}

	bool Equals(const FDungeonTileSegmentHash& Other) const
	{
		return
			TilePoint == Other.TilePoint &&
			Segment == Other.Segment;
	}
};
#if UE_BUILD_DEBUG // debuggable and slower.
uint32 GetTypeHash(const FDungeonTileSegmentHash& Thing)
{
	uint32 Hash = FCrc::MemCrc32(&Thing, sizeof(FDungeonTileSegmentHash));
	return Hash;
}
#else // optimize by inlining in shipping and development builds
FORCEINLINE uint32 GetTypeHash(const FDungeonTileSegmentHash& Thing)
{
	uint32 Hash = FCrc::MemCrc32(&Thing, sizeof(FDungeonTileSegmentHash));
	return Hash;
}
#endif


USTRUCT(BlueprintType)
struct FDungeonSegment
{
	GENERATED_BODY();

public:

	UPROPERTY(EditAnywhere)
	FGameplayTagContainer Tags;

	UPROPERTY(EditAnywhere)
	FDungeonModel Model;
};

USTRUCT(BlueprintType)
struct FDungeonTile
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FDungeonSegment> Segments;

	FDungeonTile()
	{
		for (int i=0; i < DUNGEON_TILE_SEGMENT_COUNT; i++)
			Segments.Emplace(FDungeonSegment());
	}
};

USTRUCT(BlueprintType)
struct FDungeonTileTemplate
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere)
	FDungeonModel NorthWall;

	UPROPERTY(EditAnywhere)
	FDungeonModel EastWall;

	UPROPERTY(EditAnywhere)
	FDungeonModel SouthWall;

	UPROPERTY(EditAnywhere)
	FDungeonModel WestWall;
	
	UPROPERTY(EditAnywhere)
	FDungeonModel Floor;

	UPROPERTY(EditAnywhere)
	FDungeonModel Ceiling;
};

UCLASS(NotPlaceable, BlueprintType, NotBlueprintable)
class ADungeon final : public AActor
{
	GENERATED_BODY()

public:

	ADungeon();

	virtual void OnConstruction(const FTransform& Transform) override;
	
	void RegenerateTiles();

	UPROPERTY(EditAnywhere)
	float Scale = 100.0f;

	UPROPERTY(EditAnywhere)
	FDungeonTileTemplate DefaultTileTemplate;

	UPROPERTY()
	UMaterial* SelectionMaterial;
	
	UPROPERTY()
	UMaterial* PlusIconMaterial;

	UPROPERTY()
	UMaterialInstanceDynamic* TileSelectedMaterial;

	UPROPERTY()
	UMaterialInstanceDynamic* TileUnselectedMaterial;
	
	// Tiles
	UFUNCTION(BlueprintCallable)
	bool CreateTile(FIntVector TilePoint, FDungeonTileTemplate& Template);

	UFUNCTION(BlueprintCallable)
	bool HasTile(FIntVector TilePoint);

	UFUNCTION(BlueprintCallable)
	bool GetTile(FIntVector TilePoint, FDungeonTile& Tile);

	UFUNCTION(BlueprintCallable)
	bool DeleteTile(FIntVector TilePoint);

	// Segments overrides.
	UFUNCTION(BlueprintCallable)
	bool CreateSegmentOverride(FIntVector TilePoint, EDungeonTileSegment Segment);

	UFUNCTION(BlueprintCallable)
	bool HasSegmentOverride(FIntVector TilePoint, EDungeonTileSegment Segment);

	UFUNCTION(BlueprintCallable)
	bool DeleteSegmentOverride(FIntVector TilePoint, EDungeonTileSegment Segment);

	void GetTilePoints(TArray<FIntVector>& TilePoints)
	{
		TilePoints.Empty(Tiles.Num());
		Tiles.GetKeys(TilePoints);
	};

	void GetISMCs(TArray<UInstancedStaticMeshComponent*>& _ISMCs)
	{
		_ISMCs.Empty(ISMCs.Num());
		for (const TPair<FDungeonModel, UInstancedStaticMeshComponent*>& pair : ISMCs)
			_ISMCs.Emplace(pair.Value);
	}

		
	TMap<UInstancedStaticMeshComponent*, TArray<FDungeonTileSegmentHash>> ISMCSegmentHash;
	
private:
	
	UPROPERTY(EditAnywhere)
	FDungeonModel DefaultFloor;

	UPROPERTY(EditAnywhere)
	FDungeonModel DefaultWall;

	UPROPERTY(EditAnywhere)
	FDungeonModel DefaultCeiling;

	// the actual tiles within the dungeon.
	UPROPERTY()
	TMap<FIntVector, FDungeonTile> Tiles;

	// segment override flags to force segments to be visible when there
	// is a neighbour in that direction.
	UPROPERTY()
	TSet<FDungeonSegmentOverride> SegmentOverrides;

	// all of the instanced mesh components used to render the segments within tiles.
	UPROPERTY()
	TMap<FDungeonModel, UInstancedStaticMeshComponent*> ISMCs;

	static void NormalizeSegmentOverride(FIntVector& TilePoint, EDungeonTileSegment& Segment)
	{
		// reduce all axis down to one direction on each one.
		if (Segment == EDungeonTileSegment::SOUTH_WALL)
		{
			TilePoint += SOUTH_POINT;
			Segment = EDungeonTileSegment::NORTH_WALL;
		}
		else if (Segment == EDungeonTileSegment::WEST_WALL)
		{
			TilePoint += WEST_POINT;
			Segment = EDungeonTileSegment::EAST_WALL;
		}
		else if (Segment == EDungeonTileSegment::FLOOR)
		{
			TilePoint += FLOOR_POINT;
			Segment = EDungeonTileSegment::CEILING;
		}
	}

	UInstancedStaticMeshComponent* GetInstancedStaticMeshComponent(FDungeonModel& DungeonModel);
};