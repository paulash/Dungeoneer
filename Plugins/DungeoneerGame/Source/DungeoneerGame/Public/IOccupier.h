#pragma once

#include "IOccupier.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UOccupier : public UInterface
{
	GENERATED_BODY()
};

class IOccupier
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsWalkable(FIntVector DungeonTile);
};
