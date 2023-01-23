#pragma once

#include "CoreMinimal.h"
#include "EdMode.h"
#include "Dungeon.h"

struct HDungeonTileProxy : public HHitProxy
{
	DECLARE_HIT_PROXY();

	HDungeonTileProxy(UObject* InRefObject, FIntPoint point)
		: HHitProxy(HPP_UI), RefObject(InRefObject), TilePoint(point)
	{}

	UObject* RefObject;
	FIntPoint TilePoint;
};

struct HDungeonTileWallProxy : public HHitProxy
{
	DECLARE_HIT_PROXY();

	HDungeonTileWallProxy(UObject* InRefObject, FIntPoint point, FIntPoint direction)
		: HHitProxy(HPP_UI), RefObject(InRefObject), TilePoint(point), Direction(direction)
	{}

	UObject* RefObject;
	FIntPoint TilePoint;
	FIntPoint Direction;
};

class FDungeoneerEdMode : public FEdMode
{
public:
	const static FEditorModeID EM_DungeoneerEdModeId;
public:
	FDungeoneerEdMode();
	virtual ~FDungeoneerEdMode();

	// FEdMode interface
	virtual bool ShouldDrawWidget() const override { return false; };
	virtual void Enter() override;
	virtual void Exit() override;
	void DrawHUD(FEditorViewportClient* ViewportClient, FViewport* Viewport, const FSceneView* View, FCanvas* Canvas);
	//virtual void Tick(FEditorViewportClient* ViewportClient, float DeltaTime) override;
	virtual void Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI) override;
	//virtual void ActorSelectionChangeNotify() override;
	bool UsesToolkits() const override;
	// End of FEdMode interface

	virtual bool InputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key, EInputEvent Event) override;
	virtual bool HandleClick(FEditorViewportClient* InViewportClient, HHitProxy* HitProxy, const FViewportClick& Click) override;
	virtual bool BoxSelect(FBox& InBox, bool InSelect) override;

private:

	FMaterialRenderProxy* TileSelectedMaterialProxy = NULL;
	FMaterialRenderProxy* TileUnselectedMaterialProxy = NULL;
	
	TArray<FIntPoint> SelectedTiles;
	ADungeon* LevelDungeon = NULL;
	bool ShiftHeld = false;
	bool CtrlHeld = false;
};
