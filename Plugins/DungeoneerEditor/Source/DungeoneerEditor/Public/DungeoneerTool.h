#pragma once
#include "DetailLayoutBuilder.h"
#include "UObject/GCObject.h"

class FDungeoneerEditorEdMode;

class FDungeoneerTool : public FGCObject
{
public:
	FDungeoneerTool(FDungeoneerEditorEdMode* _Mode);

	virtual FName GetToolName() { return NAME_None; };
	
	virtual void Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI) {};
	virtual void DrawHUD(FEditorViewportClient* ViewportClient, FViewport* Viewport, const FSceneView* View, FCanvas* Canvas) {};

	virtual void Tick(FEditorViewportClient* ViewportClient, float DeltaTime) {};
	virtual bool MouseMove(FEditorViewportClient* ViewportClient, FViewport* Viewport, int32 x, int32 y) { return false; };
	virtual bool HandleClick(FEditorViewportClient* InViewportClient, HHitProxy* HitProxy, const FViewportClick& Click) { return false; };

	virtual bool InputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key, EInputEvent Event) { return false; };
	virtual bool InputDelta(FEditorViewportClient* InViewportClient, FViewport* InViewport, FVector& InDrag, FRotator& InRot, FVector& InScale) { return false; };

	TSharedPtr<SCompoundWidget> GetToolPanelWidget() { return PanelWidget; };

protected:
	
	FDungeoneerEditorEdMode* Mode;
	TSharedPtr<SCompoundWidget> PanelWidget;
};