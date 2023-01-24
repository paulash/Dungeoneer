#include "DungeoneerEdMode.h"
#include "DungeoneerEdModeToolkit.h"
#include "Toolkits/ToolkitManager.h"
#include "EditorModeManager.h"
#include "EngineUtils.h"
#include "SceneManagement.h"

const FEditorModeID FDungeoneerEdMode::EM_DungeoneerEdModeId = TEXT("EM_DungeoneerEdMode");
IMPLEMENT_HIT_PROXY(HDungeonSelectionProxy, HHitProxy);

FDungeoneerEdMode::FDungeoneerEdMode()
{

}

FDungeoneerEdMode::~FDungeoneerEdMode()
{

}

void FDungeoneerEdMode::Enter()
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
	
	SegmentSelections.Empty();
	ShiftHeld = false;
	CtrlHeld = false;
	
	if (!Toolkit.IsValid() && UsesToolkits())
	{
		Toolkit = MakeShareable(new FDungeoneerEdModeToolkit);
		Toolkit->Init(Owner->GetToolkitHost());
	}
}

void FDungeoneerEdMode::Exit()
{
	if (Toolkit.IsValid())
	{
		FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
		Toolkit.Reset();
	}

	// Call base Exit method to ensure proper cleanup
	FEdMode::Exit();
}

void FDungeoneerEdMode::DrawHUD(FEditorViewportClient* ViewportClient, FViewport* Viewport, const FSceneView* View, FCanvas* Canvas)
{
	//Canvas->DrawShadowedText(100, 200, FText::FromString("test"), GEngine->GetLargeFont(), FLinearColor::White);
	FEdMode::DrawHUD(ViewportClient, Viewport, View, Canvas);
}

void FDungeoneerEdMode::Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI)
{
	if (LevelDungeon)
	{
		if (!LevelDungeon->TileSelectedMaterial) {
			LevelDungeon->TileSelectedMaterial = UMaterialInstanceDynamic::Create(LevelDungeon->SelectionMaterial, LevelDungeon);
			LevelDungeon->TileSelectedMaterial->SetVectorParameterValue("Color", FLinearColor(2,2,0,1.0f));
		}
		if (!LevelDungeon->TileUnselectedMaterial) {
			LevelDungeon->TileUnselectedMaterial = UMaterialInstanceDynamic::Create(LevelDungeon->SelectionMaterial, LevelDungeon);
			LevelDungeon->TileUnselectedMaterial->SetVectorParameterValue("Color", FLinearColor(1,1,1,0.2f));
		}
		
		for (int l=0; l < LevelDungeon->Levels.Num(); l++) {
			TArray<FIntPoint> TilePoints;
			LevelDungeon->Levels[l].Tiles.GetKeys(TilePoints);
			for (int t=0; t < TilePoints.Num(); t++)
			{
				FVector WorldPosition =
					LevelDungeon->GetActorLocation() +
					FVector(
						TilePoints[t].X * LevelDungeon->DungeonScale,
						TilePoints[t].Y * LevelDungeon->DungeonScale,
						l * LevelDungeon->DungeonLevelZ);

				/*
				FBox FloorBox = FBox(
					FVector(
						LevelDungeon->DungeonScale * -0.5f,
						LevelDungeon->DungeonScale * -0.5f,
						1.0f) + WorldPosition,
					FVector(
						LevelDungeon->DungeonScale * 0.5f,
						LevelDungeon->DungeonScale * 0.5f,
						0) + WorldPosition);

				
				bool Selected = Selections.Contains(FDungeonSelection(l, TilePoints[t]));
				PDI->SetHitProxy(new HDungeonSelectionProxy(LevelDungeon, l, TilePoints[t]));
				DrawBox(PDI, FTransform(
					FloorBox.GetCenter()).ToMatrixNoScale(),
					FloorBox.GetExtent(),
					Selected ? LevelDungeon->TileSelectedMaterial->GetRenderProxy() : LevelDungeon->TileUnselectedMaterial->GetRenderProxy(),
					SDPG_Foreground);
				PDI->SetHitProxy(NULL);
				*/
				
				for (int i=0; i < DUNGEON_DIRECTIONS.Num(); i++)
				{
					EDungeonTileSegment Segment = (EDungeonTileSegment)i;
					if ((Segment != EDungeonTileSegment::FLOOR && Segment != EDungeonTileSegment::CEILING) &&
						LevelDungeon->HasTile(l, TilePoints[t] + FIntPoint(DUNGEON_DIRECTIONS[i].X, DUNGEON_DIRECTIONS[i].Y)))
						continue;

					bool SelectedWall = SegmentSelections.Contains(FDungeonSegmentSelection(l, TilePoints[t], Segment));
					PDI->SetHitProxy(new HDungeonSelectionProxy(LevelDungeon, l, TilePoints[t], Segment));

					FVector center;
					if (Segment == EDungeonTileSegment::FLOOR)
						center = WorldPosition + FVector(0, 0, 1);
					else if (Segment == EDungeonTileSegment::CEILING)
						center = WorldPosition + FVector(0, 0, LevelDungeon->DungeonScale - 1);
					else
					{
						center = FVector(
							((DUNGEON_DIRECTIONS[i].X * 0.5f) * LevelDungeon->DungeonScale) - DUNGEON_DIRECTIONS[i].X,
							((DUNGEON_DIRECTIONS[i].Y * 0.5f) * LevelDungeon->DungeonScale) - DUNGEON_DIRECTIONS[i].Y,
							0.5f* LevelDungeon->DungeonScale) + WorldPosition;
					}

					DrawPlane10x10(PDI,
						FTransform(DUNGEON_SEGMENT_ROTATIONS[i], center).ToMatrixNoScale(),
						LevelDungeon->DungeonScale * 0.5f,
						FVector2D(0, 0),
						FVector2D(1, 1),
						SelectedWall ? LevelDungeon->TileSelectedMaterial->GetRenderProxy() : LevelDungeon->TileUnselectedMaterial->GetRenderProxy(),
						SDPG_World);

					if (CtrlHeld)
					{
						DrawPlane10x10(PDI,
							FTransform(DUNGEON_SEGMENT_ROTATIONS[i], center).ToMatrixNoScale(),
							LevelDungeon->DungeonScale * 0.5f,
							FVector2D(0, 0),
							FVector2D(1, 1),
							LevelDungeon->PlusIconMaterial->GetRenderProxy(),
							SDPG_Foreground);
					}
					PDI->SetHitProxy(NULL);
				}
			}
		}
	}
	FEdMode::Render(View, Viewport, PDI);
}

bool FDungeoneerEdMode::UsesToolkits() const
{
	return true;
}

bool FDungeoneerEdMode::InputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key,
	EInputEvent Event)
{
	if ((Key == EKeys::LeftShift || Key == EKeys::RightShift) &&
		(Event == IE_Pressed || Event == IE_Released))
			ShiftHeld = Event == IE_Pressed;

	if ((Key == EKeys::LeftControl || Key == EKeys::RightControl) &&
		(Event == IE_Pressed || Event == IE_Released))
			CtrlHeld = Event == IE_Pressed;


	if (Key == EKeys::Delete && Event == IE_Pressed)
	{
		/*
		TArray<FDungeonSegmentSelection> RemovedSelections;
		for (int i=0; i < Selections.Num(); i++)
		{
			if (Selections[i].IsTile())
			{
				LevelDungeon->DeleteTile(Selections[i].LevelIndex, Selections[i].TilePoint);
				RemovedSelections.Emplace(Selections[i]);
			}
		}
		
		// remove all the deleted tiles from selections.
		// might need to check for override walls that were removed as a result
		for (int i=0; i < RemovedSelections.Num(); i++)
			Selections.Remove(RemovedSelections[i]);
		*/
	}
	return FEdMode::InputKey(ViewportClient, Viewport, Key, Event);
}

bool FDungeoneerEdMode::HandleClick(FEditorViewportClient* InViewportClient, HHitProxy* HitProxy,
                                    const FViewportClick& Click)
{
	if (!HitProxy)
	{
		SegmentSelections.Empty();
		return FEdMode::HandleClick(InViewportClient, HitProxy, Click);
	}
	if (HitProxy->IsA(HDungeonSelectionProxy::StaticGetType()))
	{
		HDungeonSelectionProxy* SelectionProxy = (HDungeonSelectionProxy*)HitProxy;
		FDungeonSegmentSelection Selection = FDungeonSegmentSelection(
			SelectionProxy->LevelIndex,
			SelectionProxy->TilePoint,
			SelectionProxy->Segment
		);

		
		if (Selection.Segment != EDungeonTileSegment::FLOOR && Selection.Segment != EDungeonTileSegment::CEILING && Click.IsControlDown())
		{
			FIntVector direction = DUNGEON_DIRECTIONS[(int)SelectionProxy->Segment];
			LevelDungeon->CreateTile(Selection.LevelIndex, Selection.TilePoint + FIntPoint(direction.X, direction.Y));
			return true;
		}
		
		
		if (Click.IsShiftDown())
		{
			if (SegmentSelections.Contains(Selection))
				SegmentSelections.Remove(Selection);
			else
				SegmentSelections.Emplace(Selection);	
		}
		else
		{
			SegmentSelections.Empty();
			SegmentSelections.Emplace(Selection);
		}
		return true;
	}
	SegmentSelections.Empty();
	return FEdMode::HandleClick(InViewportClient, HitProxy, Click);
}

bool FDungeoneerEdMode::BoxSelect(FBox& InBox, bool InSelect)
{
	return true;
}




