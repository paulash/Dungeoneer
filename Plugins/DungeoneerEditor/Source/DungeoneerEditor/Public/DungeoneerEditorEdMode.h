// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EdMode.h"
#include "Dungeon.h"
#include "DungeoneerTool.h"

struct HDungeonSegmentProxy : public HHitProxy
{
	DECLARE_HIT_PROXY();
	
	HDungeonSegmentProxy(FIntVector tilePoint, EDungeonSegment segment)
		: HHitProxy(HPP_Foreground), TilePoint(tilePoint), Segment(segment)
	{}

	virtual EMouseCursor::Type GetMouseCursor() override
	{
		return EMouseCursor::CardinalCross;
	}
	
	FIntVector TilePoint;
	EDungeonSegment Segment;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnTemplateAdded, FName /*, TemplateName*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTemplateRemoved, FName/*, TemplateName*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnTemplateRenamed, FName/*, OldTemplateName*/, FName);
DECLARE_MULTICAST_DELEGATE(FOnTemplateUpdated);

class FDungeoneerEditorEdMode : public FEdMode
{
public:
	const static FEditorModeID EM_DungeoneerEditorEdModeId;

	FOnTemplateAdded OnTemplateAdded;
	FOnTemplateRemoved OnTemplateRemoved;
	FOnTemplateRenamed OnTemplateRenamed;
	
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
	virtual bool IsSelectionAllowed(AActor* InActor, bool bInSelection) const override;
	virtual bool UsesToolkits() const override { return true; };

	virtual bool GetCursor(EMouseCursor::Type& OutCursor) const override { return false; };
	virtual bool GetOverrideCursorVisibility(bool& bWantsOverride, bool& bHardwareCursorVisible, bool bSoftwareCursorVisible) const override { return false; };
	//virtual bool CapturedMouseMove(FEditorViewportClient* InViewportClient, FViewport* InViewport, int32 InMouseX, int32 InMouseY) override { return false; };
	//virtual bool PreConvertMouseMovement(FEditorViewportClient* InViewportClient) override { return false; };
	//virtual bool PostConvertMouseMovement(FEditorViewportClient* InViewportClient) override { return false; };
	//virtual bool Select(AActor* InActor, bool bInSelected) override { return false; };

	virtual bool InputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key, EInputEvent Event) override;
	virtual bool InputDelta(FEditorViewportClient* InViewportClient, FViewport* InViewport, FVector& InDrag, FRotator& InRot, FVector& InScale) override;

	virtual bool DisallowMouseDeltaTracking() const override { return usingTool; };
	virtual bool ShouldDrawWidget() const override { return false; };
	// End of FEdMode interface

	bool usingTool = false;
	ADungeon* LevelDungeon = NULL;

	FDungeoneerTool* CurrentTool;

	void SetCurrentTool(FName _ToolName);

	// Util Functions
	static class FDungeoneerEditorEdMode* GetEdMode();

	// edit events.
	bool AddTemplate(FName _TemplateName, FDungeonModel _Template)
	{
		if (!LevelDungeon) return false;
		if (!LevelDungeon->AddTemplate(_TemplateName, _Template)) return false;
		OnTemplateAdded.Broadcast(_TemplateName);
		return true;
	}

	bool RemoveTemplate(FName _TemplateName)
	{
		if (!LevelDungeon) return false;
		if (!LevelDungeon->RemoveTemplate(_TemplateName)) return false;
		
		OnTemplateRemoved.Broadcast(_TemplateName);
		return true;
	}

	bool RenameTemplate(FName _OldTemplateName, FName _NewTemplateName)
	{
		if (!LevelDungeon) return false;
		if (!LevelDungeon->RenameTemplate(_OldTemplateName, _NewTemplateName)) return false;
		
		OnTemplateRenamed.Broadcast(_OldTemplateName, _NewTemplateName);
		return true;
	}

	bool AddTile(FIntVector TilePoint)
	{
		if (!LevelDungeon) return false;
		LevelDungeon->CreateTile(TilePoint);
		return true;
	}

	bool RemoveTile(FIntVector TilePoint)
	{
		if (!LevelDungeon) return false;
		LevelDungeon->DeleteTile(TilePoint);
		return true;
	}

	bool UpdateTile(FIntVector TilePoint, EDungeonSegment Segment, FName NewTemplate)
	{
		if (!LevelDungeon) return false;
		LevelDungeon->SetSegmentTemplate(TilePoint, Segment, NewTemplate);
		return true;
	}

	bool RotateTile(FIntVector TilePoint, EDungeonSegment Segment, float NewRotation)
	{
		if (!LevelDungeon) return false;
		LevelDungeon->RotateSegment(TilePoint, Segment, NewRotation);
		return true;
	}
	
	bool IsCtrlDown() { return CtrlDown; };
	bool IsShiftDown() { return ShiftDown; };


	FName GetSelectedTemplate();
	
private:

	bool CtrlDown = false;
	bool ShiftDown = false;
	
	TArray<TUniquePtr<FDungeoneerTool>> Tools;
	
};
