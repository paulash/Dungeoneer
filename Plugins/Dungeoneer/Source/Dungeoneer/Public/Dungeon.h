#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/ChildActorComponent.h"
#include "GameplayTagContainer.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Containers/StaticArray.h"
#include "Dungeon.generated.h"

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

#define DUNGEON_SEGMENT_COUNT 6

const static TArray<FRotator> DUNGEON_SEGMENT_ROTATIONS = {
	FRotator(180, 90, 90),	// NORTH
	FRotator(0, 0, -90),	// EAST
	FRotator(0, 90, -90),	// SOUTH
	FRotator(180, 0, 90),	// WEST
	FRotator(0, 0, 0),		// DOWN
	FRotator(180, 0, 0),	// UP
};

#define NORTH_POINT FIntVector(1, 0, 0)
#define EAST_POINT FIntVector(0, 1, 0)
#define SOUTH_POINT FIntVector(-1, 0, 0)
#define WEST_POINT FIntVector(0, -1, 0)
#define FLOOR_POINT FIntVector(0, 0, -1)
#define CEILING_POINT FIntVector(0,0, 1)

const static TArray<FIntVector> DUNGEON_DIRECTIONS = {
	NORTH_POINT,
	EAST_POINT,
	SOUTH_POINT,
	WEST_POINT,
	FLOOR_POINT,
	CEILING_POINT
};

USTRUCT()
struct FDungeonTile
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	TArray<UMaterial*> SegmentMaterials;

	UPROPERTY(EditAnywhere)
	FGameplayTagContainer Tags;

	FDungeonTile()
	{
		SegmentMaterials.SetNum(DUNGEON_SEGMENT_COUNT);
	}
};

UCLASS(BlueprintType, NotBlueprintable)
class ADungeon final : public AActor
{
	GENERATED_BODY()

public:

	ADungeon();

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditAnywhere)
	TMap<FIntVector, FDungeonTile> Tiles;

	UPROPERTY(EditAnywhere)
	float Scale = 100.0f;

	UPROPERTY()
	UMaterial* SelectionMaterial;
	
	UPROPERTY()
	UMaterial* PlusIconMaterial;

	UPROPERTY()
	UMaterialInstanceDynamic* TileSelectedMaterial;

	UPROPERTY()
	UMaterialInstanceDynamic* TileUnselectedMaterial;

	UPROPERTY()
	UMaterialInstanceDynamic* TileHoveredUnselectedMaterial;

	UPROPERTY()
	UMaterialInstanceDynamic* TileHoveredSelectedMaterial;
	
	UPROPERTY(EditAnywhere)
	TArray<UMaterial*> DefaultSegmentMaterials;

	void CreateTile(FIntVector TilePoint);
	void DeleteTile(FIntVector TilePoint);
	void SetSegmentMaterial(FIntVector TilePoint, EDungeonDirection Segment, UMaterial* NewMaterial);
	void RegenerateTiles();

private:

	UPROPERTY()
	UStaticMesh* DungeonQuad;
	
	UPROPERTY()
	TMap<UMaterial*, UInstancedStaticMeshComponent*> ISMCs;

	UInstancedStaticMeshComponent* GetInstancedMeshComponent(UMaterial* Material);
};