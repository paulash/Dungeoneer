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

	FOnTemplateUpdated OnTemplateUpdated;
	
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

	FDungeoneerTool* CurrentTool;

	void SetCurrentTool(FName _ToolName);

	// Util Functions
	static class FDungeoneerEditorEdMode* GetEdMode();


	bool AddTemplate(FName _TemplateName, FDungeonModel _Template)
	{
		if (!LevelDungeon) return false;
		if (!LevelDungeon->AddTemplate(_TemplateName, _Template)) return false;
		OnTemplateAdded.Broadcast(_TemplateName);
		OnTemplateUpdated.Broadcast();
		return true;
	}

	bool RemoveTemplate(FName _TemplateName)
	{
		if (!LevelDungeon) return false;
		if (!LevelDungeon->RemoveTemplate(_TemplateName)) return false;
		
		OnTemplateRemoved.Broadcast(_TemplateName);
		OnTemplateUpdated.Broadcast();
		return true;
	}

	bool RenameTemplate(FName _OldTemplateName, FName _NewTemplateName)
	{
		if (!LevelDungeon) return false;
		if (!LevelDungeon->RenameTemplate(_OldTemplateName, _NewTemplateName)) return false;
		
		OnTemplateRenamed.Broadcast(_OldTemplateName, _NewTemplateName);
		OnTemplateUpdated.Broadcast();
		return true;
	}

	private:
	
	TArray<TUniquePtr<FDungeoneerTool>> Tools;
};
