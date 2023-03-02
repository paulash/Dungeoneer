#pragma once
#include "DungeoneerTool.h"

class FDungeoneerSelectTool final : public FDungeoneerTool
{
public:
	FDungeoneerSelectTool(FDungeoneerEditorEdMode* _Mode);

	virtual FName GetToolName() override { return "TOOL_SELECT"; };
	
	virtual void Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI) override;
	virtual bool HandleClick(FEditorViewportClient* InViewportClient, HHitProxy* HitProxy, const FViewportClick& Click) override;

	virtual void AddReferencedObjects(FReferenceCollector& Collector) override {};


private:
	
	TSet<FVector4> SelectedSegments;
};