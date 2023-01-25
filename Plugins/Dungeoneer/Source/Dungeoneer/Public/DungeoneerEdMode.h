#pragma once

#include "CoreMinimal.h"
#include "EdMode.h"
#include "Dungeon.h"

struct HDungeonSegmentProxy : public HHitProxy
{
	DECLARE_HIT_PROXY();
	
	HDungeonSegmentProxy(FIntVector tilePoint, EDungeonTileSegment segment)
		: HHitProxy(HPP_UI), TilePoint(tilePoint), Segment(segment)
	{}
	
	FIntVector TilePoint;
	EDungeonTileSegment Segment;
};

class FDungeoneerEdMode : public FEdMode
{
public:
	const static FEditorModeID EM_DungeoneerEdModeId;
public:
	FDungeoneerEdMode() {};
	virtual ~FDungeoneerEdMode() {};

	// FEdMode interface
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Tick(FEditorViewportClient* ViewportClient, float DeltaTime) override;
	virtual void Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI) override;

	virtual bool InputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key, EInputEvent Event) override;
	virtual bool HandleClick(FEditorViewportClient* InViewportClient, HHitProxy* HitProxy, const FViewportClick& Click) override;
	
	bool UsesToolkits() const override;
	// End of FEdMode interface

	// this mode completely hijacks the mouse, we dont want to allow selecting of other things
	virtual bool IsSelectionAllowed(AActor* InActor, bool bInSelection) const override { return false; };
	virtual bool ShouldDrawWidget() const override { return false; };

private:

	TSet<FDungeonSegmentSelection> SegmentSelections;
	ADungeon* LevelDungeon = NULL;
};
