#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "GameplayTagContainer.h"
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
struct FDungeonModel
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	TArray<UMaterial*> Materials;

	UPROPERTY(EditAnywhere)
	UStaticMesh* Mesh = NULL;
};

USTRUCT()
struct FDungeonCustomModel
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FName TemplateName;

	UPROPERTY(EditAnywhere)
	FTransform Offset;
};

USTRUCT()
struct FDungeonTile
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	TArray<FName> SegmentModels;

	UPROPERTY(EditAnywhere)
	TArray<FDungeonCustomModel> CustomModels;

	UPROPERTY(EditAnywhere)
	FGameplayTagContainer Tags;

	FDungeonTile()
	{
		SegmentModels.SetNum(DUNGEON_SEGMENT_COUNT);
	}
};

USTRUCT()
struct FDungeonPalette
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere)
	TMap<FName, FDungeonModel> Models;
};


class ADungeon;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRefreshTile, ADungeon*, Dungeon, FIntVector, TilePoint);

UCLASS(BlueprintType, NotBlueprintable)
class DUNGEONEERGAME_API ADungeon final : public AActor
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnRefreshTile OnRefreshTile;

	ADungeon();

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditAnywhere)
	TMap<FIntVector, FDungeonTile> Tiles;

	UPROPERTY(EditAnywhere)
	float Scale = 200.0f;

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
	FDungeonPalette DungeonPalette;

	UFUNCTION(BlueprintCallable)
	void CreateTile(FIntVector TilePoint);
	
	UFUNCTION(BlueprintCallable)
	void DeleteTile(FIntVector TilePoint);

	UFUNCTION(BlueprintCallable)
	void SetSegmentTemplate(FIntVector TilePoint, EDungeonDirection Segment, FName Template);

	UFUNCTION(BlueprintCallable)
	void AddTileGameplayTag(FIntVector TilePoint, FGameplayTag tag);

	UFUNCTION(BlueprintCallable)
	void RemoveTileGameplayTag(FIntVector TilePoint, FGameplayTag tag);

	UFUNCTION(BlueprintCallable)
	FGameplayTagContainer GetTileGameplayTags(FIntVector TilePoint);

	UFUNCTION(BlueprintPure)
	FIntVector GetWorldLocationToTilePoint(FVector WorldLocation)
	{
		return FIntVector(
			WorldLocation.X / Scale,
			WorldLocation.Y / Scale,
			WorldLocation.Z / Scale
		);	
	};

	UFUNCTION(BlueprintPure)
	FVector GetTilePointToWorldLocation(FIntVector TilePoint)
	{
		return FVector(
			(TilePoint.X * Scale),
			(TilePoint.Y * Scale),
			(TilePoint.Z * Scale) 
		);
	}
	
	void RegenerateTiles();

private:

	UPROPERTY()
	UStaticMesh* DungeonQuad;
	
	UPROPERTY()
	TMap<FName, UInstancedStaticMeshComponent*> ISMCs;

	UPROPERTY()
	TMap<UInstancedStaticMeshComponent*, FName> ISMCValues;

	UInstancedStaticMeshComponent* GetInstancedMeshComponent(FName TemplateName);
};