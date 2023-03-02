#pragma once
#include "DungeoneerTool.h"

class FDungeoneerSelectTool final : public FDungeoneerTool
{
public:
	FDungeoneerSelectTool(FDungeoneerEditorEdMode* _Mode) : FDungeoneerTool(_Mode) {}

	virtual FName GetToolName() override { return "TOOL_SELECT"; };
	
	virtual void DrawHUD(FEditorViewportClient* ViewportClient, FViewport* Viewport, const FSceneView* View, FCanvas* Canvas) override;
	virtual void Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI) override;
	virtual bool HandleClick(FEditorViewportClient* InViewportClient, HHitProxy* HitProxy, const FViewportClick& Click) override;

	virtual TSharedPtr<SCompoundWidget> GenerateToolPanel() override;
	
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override {};


private:
	
	TSet<FVector4> SelectedSegments;
};