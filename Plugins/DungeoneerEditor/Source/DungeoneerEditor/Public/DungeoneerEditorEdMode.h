// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EdMode.h"
#include "Dungeon.h"
#include "DungeoneerTool.h"
#include "Delegates/DelegateSignatureImpl.inl"

struct HDungeonSegmentProxy : public HHitProxy
{
	DECLARE_HIT_PROXY();
	
	HDungeonSegmentProxy(FIntVector tilePoint, EDungeonDirection segment)
		: HHitProxy(HPP_UI), TilePoint(tilePoint), Segment(segment)
	{}
	
	FIntVector TilePoint;
	EDungeonDirection Segment;
};

class FDungeoneerEditorEdMode : public FEdMode
{
public:
	const static FEditorModeID EM_DungeoneerEditorEdModeId;

	
public:
	FDungeoneerEditorEdMode();
	virtual ~FDungeoneerEditorEdMode();

	// FEdMode interface
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Tick(FEditorViewportClient* ViewportClient, float DeltaTime) override;
	virtual bool MouseMove(FEditorViewportClient* ViewportClient, FViewport* Viewport, int32 x, int32 y) override;
	virtual void Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI) override;
	virtual void DrawHUD(FEditorViewportClient* ViewportClient, FViewport* Viewport, const FSceneView* View, FCanvas* Canvas) override;
	virtual bool HandleClick(FEditorViewportClient* InViewportClient, HHitProxy* HitProxy, const FViewportClick& Click) override;
	virtual bool IsSelectionAllowed(AActor* InActor, bool bInSelection) const override;;
	virtual bool UsesToolkits() const override { return true; };

	virtual bool GetCursor(EMouseCursor::Type& OutCursor) const override { return false; };
	virtual bool GetOverrideCursorVisibility(bool& bWantsOverride, bool& bHardwareCursorVisible, bool bSoftwareCursorVisible) const override { return false; };

	virtual bool InputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key, EInputEvent Event) override;
	virtual bool InputDelta(FEditorViewportClient* InViewportClient, FViewport* InViewport, FVector& InDrag, FRotator& InRot, FVector& InScale) override;

	virtual bool DisallowMouseDeltaTracking() const override { return usingTool; };
	virtual bool ShouldDrawWidget() const override { return false; };
	// End of FEdMode interface

	bool usingTool = false;
	ADungeon* LevelDungeon = NULL;
	FName SelectedTemplate = NAME_None;

	FDungeoneerTool* CurrentTool;

	void SetCurrentTool(FName _ToolName);
	
private:
	
	TArray<TUniquePtr<FDungeoneerTool>> Tools;
};
