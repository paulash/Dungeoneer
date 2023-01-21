#pragma once

#include "CoreMinimal.h"
#include "EdMode.h"
#include "Dungeon.h"

class FDungeoneerEdMode : public FEdMode
{
public:
	const static FEditorModeID EM_DungeoneerEdModeId;
public:
	FDungeoneerEdMode();
	virtual ~FDungeoneerEdMode();

	// FEdMode interface
	virtual void Enter() override;
	virtual void Exit() override;
	//virtual void Tick(FEditorViewportClient* ViewportClient, float DeltaTime) override;
	//virtual void Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI) override;
	//virtual void ActorSelectionChangeNotify() override;
	bool UsesToolkits() const override;
	// End of FEdMode interface

private:
	
	TArray<FIntPoint> selectedTiles;
	ADungeon* LevelDungeon = NULL;
};
