#pragma once
#include "DungeoneerTool.h"

class FDungeoneerPaintTool final : public FDungeoneerTool
{
public:
	FDungeoneerPaintTool(FDungeoneerEditorEdMode* _Mode);

	virtual FName GetToolName() override { return "TOOL_PAINT"; };
	virtual void Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI) override;
	virtual bool HandleClick(FEditorViewportClient* InViewportClient, HHitProxy* HitProxy, const FViewportClick& Click) override;
	virtual bool MouseMove(FEditorViewportClient* ViewportClient, FViewport* Viewport, int32 x, int32 y) override;
	
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override {};

private:
	
	bool ValidHover = false;
	FVector4 HoveredSegment;
};