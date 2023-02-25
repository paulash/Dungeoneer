// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EdMode.h"
#include "Dungeon.h"

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
	virtual bool MouseMove(FEditorViewportClient* ViewportClient, FViewport* Viewport, int32 x, int32 y) override;
	virtual void Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI) override;
	virtual void DrawHUD(FEditorViewportClient* ViewportClient, FViewport* Viewport, const FSceneView* View, FCanvas* Canvas) override;
	virtual bool HandleClick(FEditorViewportClient* InViewportClient, HHitProxy* HitProxy, const FViewportClick& Click) override;
	virtual bool IsSelectionAllowed(AActor* InActor, bool bInSelection) const override;;
	virtual bool UsesToolkits() const override { return true; };
	// End of FEdMode interface

	ADungeon* LevelDungeon = NULL;
	FName SelectedTemplate = NAME_None;
	TSet<FIntVector4> SelectedSegments;
};
