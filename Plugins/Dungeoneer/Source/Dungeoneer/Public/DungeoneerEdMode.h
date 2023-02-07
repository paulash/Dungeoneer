#pragma once

#include "CoreMinimal.h"
#include "Dungeon.h"
#include "DungeoneerEdModeToolkit.h"
#include "EdMode.h"


struct HDungeonSegmentProxy : public HHitProxy
{
	DECLARE_HIT_PROXY();
	
	HDungeonSegmentProxy(FIntVector tilePoint, EDungeonDirection segment)
		: HHitProxy(HPP_UI), TilePoint(tilePoint), Segment(segment)
	{}
	
	FIntVector TilePoint;
	EDungeonDirection Segment;
};

class FDungeoneerEdMode : public FEdMode
{
public:
	const static FEditorModeID EM_DungeoneerEdModeId;
public:
	FDungeoneerEdMode();
	virtual ~FDungeoneerEdMode();

	// FEdMode interface
	virtual void Enter() override;
	virtual void Exit() override;
	//virtual void Tick(FEditorViewportClient* ViewportClient, float DeltaTime) override;
	virtual void Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI) override;
	virtual bool HandleClick(FEditorViewportClient* InViewportClient, HHitProxy* HitProxy, const FViewportClick& Click) override;
	//virtual void ActorSelectionChangeNotify() override;
	virtual bool IsSelectionAllowed(AActor* InActor, bool bInSelection) const override { return false; };
	bool UsesToolkits() const override;
	// End of FEdMode interface

	FDungeoneerEdModeToolkit* GetToolKit()
	{
		return (FDungeoneerEdModeToolkit*)Toolkit.Get();
	};
	
	ADungeon* LevelDungeon = NULL;

	TSet<FIntVector4> SelectedSegments; 
};
