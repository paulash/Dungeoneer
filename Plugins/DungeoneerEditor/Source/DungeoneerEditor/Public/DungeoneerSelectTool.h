#pragma once
#include "DungeoneerTool.h"

class FDungeoneerSelectTool final : public FDungeoneerTool
{
public:
	FDungeoneerSelectTool(FDungeoneerEditorEdMode* _Mode) : FDungeoneerTool(_Mode) {}
	
	virtual void DrawHUD(FEditorViewportClient* ViewportClient, FViewport* Viewport, const FSceneView* View, FCanvas* Canvas) override;
	virtual void Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI) override;

	
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override {};
};