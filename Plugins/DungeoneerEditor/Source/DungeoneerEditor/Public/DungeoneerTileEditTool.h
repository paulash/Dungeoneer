#pragma once
#include "DungeoneerTool.h"

class FDungeoneerTileEditTool final : public FDungeoneerTool
{
public:
	FDungeoneerTileEditTool(FDungeoneerEditorEdMode* _Mode);

	virtual void Shutdown() override;

	virtual FName GetToolName() override { return "TOOL_EDIT"; };
	
	virtual void Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI) override;
	virtual bool HandleClick(FEditorViewportClient* InViewportClient, HHitProxy* HitProxy, const FViewportClick& Click) override;
	virtual bool MouseMove(FEditorViewportClient* ViewportClient, FViewport* Viewport, int32 x, int32 y) override;
	virtual bool InputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key, EInputEvent Event) override;

	virtual void AddReferencedObjects(FReferenceCollector& Collector) override {};

private:

	bool HoveredSegmentValid = false;
	FVector4 HoveredSegment;
	TArray<FVector4> SelectedSegments;
};