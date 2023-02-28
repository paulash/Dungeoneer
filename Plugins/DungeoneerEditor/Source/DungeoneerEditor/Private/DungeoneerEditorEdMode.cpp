
#include "DungeoneerEditorEdMode.h"
#include "DungeoneerEditorEdModeToolkit.h"
#include "DungeoneerPaintTool.h"
#include "Toolkits/ToolkitManager.h"
#include "EditorModeManager.h"
#include "EngineUtils.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "DungeoneerSelectTool.h"

IMPLEMENT_HIT_PROXY(HDungeonSegmentProxy, HHitProxy);
const FEditorModeID FDungeoneerEditorEdMode::EM_DungeoneerEditorEdModeId = TEXT("EM_DungeoneerEditorEdMode");

FDungeoneerEditorEdMode::FDungeoneerEditorEdMode()
{
	Tools.Empty();

	auto Tool_Select = MakeUnique<FDungeoneerSelectTool>(this);
	Tools.Emplace(MoveTemp(Tool_Select));

	auto Tool_Paint = MakeUnique<FDungeoneerPaintTool>(this);
	Tools.Emplace(MoveTemp(Tool_Paint));
	
	CurrentTool = Tools[0].Get();
}

FDungeoneerEditorEdMode::~FDungeoneerEditorEdMode()
{
}

void FDungeoneerEditorEdMode::Enter()
{
	FEdMode::Enter();

	LevelDungeon = NULL;
	UWorld* World = GEditor->GetEditorWorldContext().World();
	for (TActorIterator<ADungeon> It(World); It; ++It)
	{
		ADungeon* dungeon = (*It);
		if (!LevelDungeon)
		{
			LevelDungeon = dungeon;
			break;
		}
	}
	if (!LevelDungeon)
		LevelDungeon = World->SpawnActor<ADungeon>(ADungeon::StaticClass());
	

	if (LevelDungeon)
	{
		if (!LevelDungeon->TileSelectedMaterial) {
			LevelDungeon->TileSelectedMaterial = UMaterialInstanceDynamic::Create(LevelDungeon->SelectionMaterial, LevelDungeon);
			LevelDungeon->TileSelectedMaterial->SetVectorParameterValue("Color", FLinearColor(2,2,0,0.2f));
		}
		if (!LevelDungeon->TileUnselectedMaterial) {
			LevelDungeon->TileUnselectedMaterial = UMaterialInstanceDynamic::Create(LevelDungeon->SelectionMaterial, LevelDungeon);
			LevelDungeon->TileUnselectedMaterial->SetVectorParameterValue("Color", FLinearColor(1,1,1,0.2f));
		}
		if (!LevelDungeon->TileHoveredSelectedMaterial) {
			LevelDungeon->TileHoveredSelectedMaterial = UMaterialInstanceDynamic::Create(LevelDungeon->SelectionMaterial, LevelDungeon);
			LevelDungeon->TileHoveredSelectedMaterial->SetVectorParameterValue("Color", FLinearColor(2,2,0,0.9f));
		}
		if (!LevelDungeon->TileHoveredUnselectedMaterial) {
			LevelDungeon->TileHoveredUnselectedMaterial = UMaterialInstanceDynamic::Create(LevelDungeon->SelectionMaterial, LevelDungeon);
			LevelDungeon->TileHoveredUnselectedMaterial->SetVectorParameterValue("Color", FLinearColor(1, 1, 1,0.9f));
		}
	}
	
	if (!Toolkit.IsValid() && UsesToolkits())
	{
		Toolkit = MakeShareable(new FDungeoneerEditorEdModeToolkit);
		Toolkit->Init(Owner->GetToolkitHost());
	}
}

void FDungeoneerEditorEdMode::Exit()
{
	if (Toolkit.IsValid())
	{
		FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
		Toolkit.Reset();
	}

	// Call base Exit method to ensure proper cleanup
	FEdMode::Exit();
}

void FDungeoneerEditorEdMode::Tick(FEditorViewportClient* ViewportClient, float DeltaTime)
{
	ViewportClient->Viewport->CaptureMouse(usingTool);
	FEdMode::Tick(ViewportClient, DeltaTime);
}

bool FDungeoneerEditorEdMode::MouseMove(FEditorViewportClient* ViewportClient, FViewport* Viewport, int32 x, int32 y)
{
	return CurrentTool->MouseMove(ViewportClient, Viewport, x, y);
}

void FDungeoneerEditorEdMode::Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI)
{
	FEdMode::Render(View, Viewport, PDI);
	if (!LevelDungeon) return;
	CurrentTool->Render(View, Viewport, PDI);
}

void FDungeoneerEditorEdMode::DrawHUD(FEditorViewportClient* ViewportClient, FViewport* Viewport,
	const FSceneView* View, FCanvas* Canvas)
{
	CurrentTool->DrawHUD(ViewportClient, Viewport, View, Canvas);
}

bool FDungeoneerEditorEdMode::HandleClick(FEditorViewportClient* InViewportClient, HHitProxy* HitProxy,
                                          const FViewportClick& Click)
{
	return CurrentTool->HandleClick(InViewportClient, HitProxy, Click);
	/*
	if (!HitProxy)
	{
		SelectedSegments.Empty();
		return FEdMode::HandleClick(InViewportClient, HitProxy, Click);
	}
	if (HitProxy->IsA(HDungeonSegmentProxy::StaticGetType()))
	{
		HDungeonSegmentProxy* SegmentProxy = (HDungeonSegmentProxy*)HitProxy;
		if (Click.IsShiftDown() && Click.IsControlDown())
		{
			LevelDungeon->DeleteTile(SegmentProxy->TilePoint);
		}
		else if (Click.IsShiftDown())
		{
			// paint template.
			LevelDungeon->SetSegmentTemplate(SegmentProxy->TilePoint, SegmentProxy->Segment, SelectedTemplate);
		}
		else if (Click.IsControlDown())
		{
			LevelDungeon->CreateTile(
				SegmentProxy->TilePoint + DUNGEON_DIRECTIONS[(int)SegmentProxy->Segment]);
		}
		else
		{
			FIntVector4 Selection = FIntVector4(
				SegmentProxy->TilePoint.X,
				SegmentProxy->TilePoint.Y,
				SegmentProxy->TilePoint.Z,
				(int)SegmentProxy->Segment);
			
			if (!Click.IsShiftDown())
				SelectedSegments.Empty();

			if (!SelectedSegments.Contains(Selection))
			{
				//GetToolKit()->SelectTileSegment(Selection);
				SelectedSegments.Emplace(Selection);
			}
		}
		return true;
	}
	SelectedSegments.Empty();
	return FEdMode::HandleClick(InViewportClient, HitProxy, Click);
	*/
}

bool FDungeoneerEditorEdMode::IsSelectionAllowed(AActor* InActor, bool bInSelection) const
{
	return false;
}

bool FDungeoneerEditorEdMode::InputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key,
	EInputEvent Event)
{
	return CurrentTool->InputKey(ViewportClient, Viewport, Key, Event);
}

bool FDungeoneerEditorEdMode::InputDelta(FEditorViewportClient* InViewportClient, FViewport* InViewport,
	FVector& InDrag, FRotator& InRot, FVector& InScale)
{
	return CurrentTool->InputDelta(InViewportClient, InViewport, InDrag, InRot, InScale);
}

void FDungeoneerEditorEdMode::SetCurrentTool(FName _ToolName)
{
	for (int i=0; i < Tools.Num(); i++)
	{
		if (Tools[i]->GetToolName() == _ToolName)
		{
			CurrentTool = Tools[i].Get();
			FDungeoneerEditorEdModeToolkit* _toolkit = (FDungeoneerEditorEdModeToolkit*)Toolkit.Get();
			if (_toolkit)
			{
				SDungeoneerEditorWidget* widget = (SDungeoneerEditorWidget*)_toolkit->GetInlineContent().Get();
				widget->OnCurrentToolChange();
			}
			return;
		}
	}
	
}




