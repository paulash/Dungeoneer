#pragma once
#include "UObject/GCObject.h"

class FDungeoneerEditorEdMode;

class FDungeoneerTool : public FGCObject
{
public:
	FDungeoneerTool(FDungeoneerEditorEdMode* _Mode);
	
	virtual void Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI) {};
	virtual void DrawHUD(FEditorViewportClient* ViewportClient, FViewport* Viewport, const FSceneView* View, FCanvas* Canvas) {};

protected:
	FDungeoneerEditorEdMode* Mode;
};