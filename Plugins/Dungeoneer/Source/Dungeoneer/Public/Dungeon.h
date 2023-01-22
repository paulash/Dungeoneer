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
	bool GetTile(int FloorIndex, FIntPoint Point, FDungeonFloor& OutFloor, FDungeonTile& OutTile)
	{
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
		return CreateTile(floorIndex, FIntPoint(0,0), floor, tile); 
	}

	UFUNCTION(BlueprintCallable)
	bool CreateTile(int FloorIndex, FIntPoint Point, FDungeonFloor& OutFloor, FDungeonTile& OutTile)
	{
		// no such floor.
		if (!GetFloor(FloorIndex, OutFloor))
			return false;

		// tile already exists.
		if (GetTile(FloorIndex, Point, OutFloor, OutTile))
			return false;

		OutTile = FDungeonTile();
		
		OutTile.Segments.Emplace(FDungeonTileSegment());
		OutTile.Segments.Emplace(FDungeonTileSegment());
		OutTile.Segments.Emplace(FDungeonTileSegment());
		OutTile.Segments.Emplace(FDungeonTileSegment());
		OutTile.Segments.Emplace(FDungeonTileSegment());
		OutTile.Segments.Emplace(FDungeonTileSegment());
		
		Floors[FloorIndex].Tiles.Emplace(Point, OutTile);
		return true;
	}
	
private:

	UPROPERTY()
	TMap<FMeshMaterialPair, UInstancedStaticMeshComponent*> InstancedStaticMeshComponents;

	UInstancedStaticMeshComponent* GetInstancedStaticMeshComponent(UMaterial* Material, UStaticMesh* Mesh);

};