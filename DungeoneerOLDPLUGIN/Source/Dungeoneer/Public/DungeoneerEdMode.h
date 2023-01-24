#pragma once

#include "CoreMinimal.h"
#include "EdMode.h"
#include "Dungeon.h"

struct HDungeonSelectionProxy : public HHitProxy
{
	DECLARE_HIT_PROXY();

	HDungeonSelectionProxy(UObject* InRefObject, int levelIndex, FIntPoint point)
		: HHitProxy(HPP_UI), RefObject(InRefObject), LevelIndex(levelIndex), TilePoint(point), Segment(EDungeonTileSegment::FLOOR)
	{}
	
	HDungeonSelectionProxy(UObject* InRefObject, int levelIndex, FIntPoint point, EDungeonTileSegment segment)
		: HHitProxy(HPP_UI), RefObject(InRefObject), LevelIndex(levelIndex), TilePoint(point), Segment(segment)
	{}

	UObject* RefObject;

	int LevelIndex;
	FIntPoint TilePoint;
	EDungeonTileSegment Segment;
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
	virtual bool IsSelectionAllowed(AActor* InActor, bool bInSelection) const override
	{
		return false;
	}

private:

	FMaterialRenderProxy* TileSelectedMaterialProxy = NULL;
	FMaterialRenderProxy* TileUnselectedMaterialProxy = NULL;
	
	TArray<FDungeonSegmentSelection> SegmentSelections;
	ADungeon* LevelDungeon = NULL;
	bool ShiftHeld = false;
	bool CtrlHeld = false;
};
