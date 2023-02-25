
#include "DungeoneerEditorEdMode.h"
#include "DungeoneerEditorEdModeToolkit.h"
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
	InitializeTool_Select();
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
	HHitProxy* HitProxy = Viewport->GetHitProxy(x, y);
	if (HitProxy && HitProxy->IsA(HDungeonSegmentProxy::StaticGetType()))
	{
		HDungeonSegmentProxy* SegmentProxy = (HDungeonSegmentProxy*)HitProxy;
		
	}
	return FEdMode::MouseMove(ViewportClient, Viewport, x, y);
}

void FDungeoneerEditorEdMode::Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI)
{
	FEdMode::Render(View, Viewport, PDI);
	//MousePosition.X = FMath::Clamp(Viewport->GetMouseX(), 0, Viewport->GetSizeXY().X-1);
	//MousePosition.Y = FMath::Clamp(Viewport->GetMouseY(), 0, Viewport->GetSizeXY().Y-1);
	
	if (!LevelDungeon) return;
	
	TArray<FIntVector> TilePoints;
	LevelDungeon->Tiles.GetKeys(TilePoints);
	for (int i=0; i < TilePoints.Num(); i++)
	{
		FDungeonTile Tile = LevelDungeon->Tiles[TilePoints[i]];
		for (int s=0; s < DUNGEON_SEGMENT_COUNT; s++)
		{
			if (LevelDungeon->Tiles.Contains(TilePoints[i] + DUNGEON_DIRECTIONS[s]))
				continue;
			
			FVector WorldPosition =
				LevelDungeon->GetActorLocation() +
				FVector(
					TilePoints[i].X * LevelDungeon->Scale,
					TilePoints[i].Y * LevelDungeon->Scale,
					(TilePoints[i].Z * LevelDungeon->Scale) + (LevelDungeon->Scale * 0.5));
			
			FVector center = FVector(
			((DUNGEON_DIRECTIONS[s].X * 0.5f) * LevelDungeon->Scale) - DUNGEON_DIRECTIONS[s].X,
			((DUNGEON_DIRECTIONS[s].Y * 0.5f) * LevelDungeon->Scale) - DUNGEON_DIRECTIONS[s].Y,
			((DUNGEON_DIRECTIONS[s].Z * 0.5f) * LevelDungeon->Scale) - DUNGEON_DIRECTIONS[s].Z);
			center -= FVector(
				DUNGEON_DIRECTIONS[s].X,
				DUNGEON_DIRECTIONS[s].Y,
				DUNGEON_DIRECTIONS[s].Z);

			FIntVector4 SegmentPoint = FIntVector4(
				TilePoints[i].X,
				TilePoints[i].Y,
				TilePoints[i].Z,
				s);

			bool selected = SelectedSegments.Contains(SegmentPoint);
			bool hovered = false;
			
			PDI->SetHitProxy(new HDungeonSegmentProxy(TilePoints[i], (EDungeonDirection)s));
			FMaterialRenderProxy* ProxyMaterial = selected ?
					LevelDungeon->TileSelectedMaterial->GetRenderProxy() :
					LevelDungeon->TileUnselectedMaterial->GetRenderProxy();;
			if (hovered)
			{
				ProxyMaterial = selected ?
					LevelDungeon->TileHoveredSelectedMaterial->GetRenderProxy() :
					LevelDungeon->TileHoveredUnselectedMaterial->GetRenderProxy();	
			}

			DrawPlane10x10(PDI,
				FTransform(
					DUNGEON_SEGMENT_ROTATIONS[s],
					center + WorldPosition).ToMatrixNoScale(),
				(LevelDungeon->Scale * 0.5f) - 1,
				FVector2D(0, 0),
				FVector2D(1, 1),
				ProxyMaterial,
				SDPG_World);
		}
	}
}

void FDungeoneerEditorEdMode::DrawHUD(FEditorViewportClient* ViewportClient, FViewport* Viewport,
	const FSceneView* View, FCanvas* Canvas)
{
	Canvas->DrawShadowedText(10, 100, FText::FromString("Test Text"), GEditor->EditorFont, FLinearColor::Red);
	CurrentTool->DrawHUD(ViewportClient, Viewport, View, Canvas);
}

bool FDungeoneerEditorEdMode::HandleClick(FEditorViewportClient* InViewportClient, HHitProxy* HitProxy,
                                          const FViewportClick& Click)
{
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
}

bool FDungeoneerEditorEdMode::IsSelectionAllowed(AActor* InActor, bool bInSelection) const
{
	return false;
}

bool FDungeoneerEditorEdMode::InputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key,
	EInputEvent Event)
{
	usingTool = IsCtrlDown(Viewport) || IsShiftDown(Viewport);
	//Viewport->CaptureMouse(usingTool);

	
	
	if (usingTool && Event == IE_Released && Key == EKeys::LeftMouseButton)
	{
		Viewport->SetPreCaptureMousePosFromSlateCursor();
	}
	
	return FEdMode::InputKey(ViewportClient, Viewport, Key, Event);
}

bool FDungeoneerEditorEdMode::InputDelta(FEditorViewportClient* InViewportClient, FViewport* InViewport,
	FVector& InDrag, FRotator& InRot, FVector& InScale)
{
	if (usingTool) return true;
	return FEdMode::InputDelta(InViewportClient, InViewport, InDrag, InRot, InScale);
}

bool FDungeoneerEditorEdMode::StartTracking(FEditorViewportClient* InViewportClient, FViewport* InViewport)
{
	return FEdMode::StartTracking(InViewportClient, InViewport);
}

bool FDungeoneerEditorEdMode::EndTracking(FEditorViewportClient* InViewportClient, FViewport* InViewport)
{
	return FEdMode::EndTracking(InViewportClient, InViewport);
}

void FDungeoneerEditorEdMode::InitializeTool_Select()
{
	auto Tool_Select = MakeUnique<FDungeoneerSelectTool>(this);
	Tools.Emplace(MoveTemp(Tool_Select));
}





