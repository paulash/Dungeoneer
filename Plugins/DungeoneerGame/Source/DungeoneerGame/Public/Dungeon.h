#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "GameplayTagContainer.h"
#include "Dungeon.generated.h"

UENUM(BlueprintType)
enum class EDungeonSegment : uint8
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

const static TArray<FVector> DUNGEON_SEGMENT_NORMAL = {
	FVector(0, 1, 0),
	FVector(0, 1, 0),
	FVector(0, -1 ,0),
	FVector(0, -1, 0),
	FVector(0, 0, -1),
	FVector(0, 0, 1)
};

#define NORTH_POINT FIntVector(1, 0, 0)
#define EAST_POINT FIntVector(0, 1, 0)
#define SOUTH_POINT FIntVector(-1, 0, 0)
#define WEST_POINT FIntVector(0, -1, 0)
#define FLOOR_POINT FIntVector(0, 0, -1)
#define CEILING_POINT FIntVector(0,0, 1)

const static TArray<FIntVector> DUNGEON_SEGMENT_OFFSETS = {
	NORTH_POINT,
	EAST_POINT,
	SOUTH_POINT,
	WEST_POINT,
	FLOOR_POINT,
	CEILING_POINT
};

USTRUCT()
struct FDungeonBatchedInstance
{
	GENERATED_BODY()

public:

	UPROPERTY()
	TArray<FTransform> Transforms;

	UPROPERTY()
	TArray<FIntVector> TilePoints;

	// if this is a negative value, its a custom model index.
	// if its positive, its a direction and normal segment.
	UPROPERTY()
	TArray<int> Segments;
	
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

USTRUCT(BlueprintType)
struct FDungeonTile
{
	GENERATED_BODY()

public:

	UPROPERTY()
	TArray<FName> SegmentModels;

	// rotations can only be applied to the normal of the segment, so its only 'yaw' from within the segments space.
	// IE it spins in the place.
	UPROPERTY()
	TArray<float> SegmentRotation;

	UPROPERTY(EditAnywhere)
	FGameplayTagContainer Tags;

	FDungeonTile()
	{
		SegmentModels.SetNum(DUNGEON_SEGMENT_COUNT);
		SegmentRotation.SetNum(DUNGEON_SEGMENT_COUNT);
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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTileTagsChanged, ADungeon*, Dungeon, FIntVector, TilePoint);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGenerated, ADungeon*, Dungeon);


UCLASS(BlueprintType, NotBlueprintable)
class DUNGEONEERGAME_API ADungeon final : public AActor
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnRefreshTile OnRefreshTile;

	UPROPERTY(BlueprintAssignable)
	FOnGenerated OnGenerated;

	UPROPERTY(BlueprintAssignable)
	FOnTileTagsChanged OnTileTagsChanged;

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

	UFUNCTION(BlueprintPure)
	bool GetTile(FIntVector TilePoint, FDungeonTile& Tile)
	{
		Tile = Tiles.FindRef(TilePoint);
		return Tiles.Contains(TilePoint);
	}

	UFUNCTION(BlueprintPure)
	FIntVector DirectionToVector(EDungeonSegment Direction)
	{
		return DUNGEON_SEGMENT_OFFSETS[(int)Direction];
	};

	UFUNCTION(BlueprintPure)
	FRotator DirectionToRotator(EDungeonSegment Direction)
	{
		return FRotator(0, 0, (int)Direction * 90);
	}

	UFUNCTION(BlueprintPure)
	FIntVector GetTileVectorInDirection(FIntVector Center, EDungeonSegment FacingDirection, EDungeonSegment SampleDirection)
	{
		return FIntVector(0,0,0);
	};

	UFUNCTION(BlueprintCallable)
	void SetSegmentTemplate(FIntVector TilePoint, EDungeonSegment Segment, FName Template);

	UFUNCTION(BlueprintCallable)
	void RotateSegment(FIntVector TilePoint, EDungeonSegment Segment, float rotation);

	UFUNCTION(BlueprintCallable)
	void AddTileGameplayTag(FIntVector TilePoint, FGameplayTag tag);

	UFUNCTION(BlueprintCallable)
	void RemoveTileGameplayTag(FIntVector TilePoint, FGameplayTag tag);

	UFUNCTION(BlueprintPure)
	bool TileHasTag(FIntVector TilePoint, FGameplayTag tag)
	{
		if (!Tiles.Contains(TilePoint)) return false;
		return Tiles[TilePoint].Tags.HasTag(tag);
	};

	UFUNCTION(BlueprintPure)
	FIntVector WorldLocationToTilePoint(FVector WorldLocation)
	{
		return FIntVector(
			WorldLocation.X / Scale,
			WorldLocation.Y / Scale,
			WorldLocation.Z / Scale
		);	
	};

	UFUNCTION(BlueprintPure)
	FVector TilePointToWorldLocation(FIntVector TilePoint)
	{
		return FVector(
			(TilePoint.X * Scale),
			(TilePoint.Y * Scale),
			(TilePoint.Z * Scale) 
		);
	}
	
	void RegenerateTiles();

	bool AddTemplate(FName TemplateName, FDungeonModel Template)
	{
		if (DungeonPalette.Models.Contains(TemplateName)) return false;
		DungeonPalette.Models.Emplace(TemplateName, Template);
		RegenerateTiles();
		return true;
	};

	bool RemoveTemplate(FName TemplateName)
	{
		if (!DungeonPalette.Models.Contains(TemplateName)) return false;
		DungeonPalette.Models.Remove(TemplateName);
		RegenerateTiles();
		return true;
	};

	bool RenameTemplate(FName OldTemplateName, FName NewTemplateName)
	{
		if (!DungeonPalette.Models.Contains(OldTemplateName)) return false;
		if (DungeonPalette.Models.Contains(NewTemplateName)) return false;
		FDungeonModel _template = DungeonPalette.Models[OldTemplateName];
		
		RemoveTemplate(OldTemplateName);
		AddTemplate(NewTemplateName, _template);

		// replace all the references to the old template name, to the new template name.
		TArray<FIntVector> TilePoints;
		Tiles.GenerateKeyArray(TilePoints);
		for (int i=0; i < TilePoints.Num(); i++)
		{
			for (int s=0; s < DUNGEON_SEGMENT_COUNT; s++)
			{
				if (Tiles[TilePoints[i]].SegmentModels[s] == OldTemplateName)
					Tiles[TilePoints[i]].SegmentModels[s] = NewTemplateName;
			}
		}
		
		RegenerateTiles();
		return true;
	};

	bool GetHitInfo(const HInstancedStaticMeshInstance* hitProxy, FIntVector& TilePoint, EDungeonSegment& Direction, int& CustomModelIndex)
	{
		if (!ISMCValues.Contains(hitProxy->Component)) return false;
		if (hitProxy->Component->IsRenderStateDirty()) return false; // currently rendering.
		TilePoint = FIntVector(
				hitProxy->Component->PerInstanceSMCustomData[(hitProxy->InstanceIndex * 4)],
				hitProxy->Component->PerInstanceSMCustomData[(hitProxy->InstanceIndex * 4) + 1],
				hitProxy->Component->PerInstanceSMCustomData[(hitProxy->InstanceIndex * 4) + 2]
		);

		CustomModelIndex = -1;
		Direction = EDungeonSegment::NORTH;
		
		int segment = hitProxy->Component->PerInstanceSMCustomData[(hitProxy->InstanceIndex * 4)+3];
		if (segment >= 0)
			Direction = (EDungeonSegment)segment;
		else
			CustomModelIndex = -segment;
		return true;
	}
	
private:

	UPROPERTY()
	UStaticMesh* DungeonQuad;

	UPROPERTY()
	TMap<FName, UInstancedStaticMeshComponent*> ISMCs;

	UPROPERTY()
	TMap<UInstancedStaticMeshComponent*, FName> ISMCValues;

	UInstancedStaticMeshComponent* GetInstancedMeshComponent(FName TemplateName);
};