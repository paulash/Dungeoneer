#pragma once

#include "CoreMinimal.h"
#include "EdMode.h"

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
	bool UsesToolkits() const override;
	// End of FEdMode interface

	// this mode completely hijacks the mouse, we dont want to allow selecting of other things
	virtual bool IsSelectionAllowed(AActor* InActor, bool bInSelection) const override
	{
		return false;
	};
};
