#include "DungeoneerEdMode.h"
#include "DungeoneerEdModeToolkit.h"
#include "Toolkits/ToolkitManager.h"
#include "EditorModeManager.h"
#include "EngineUtils.h"
#include "SceneManagement.h"

const FEditorModeID FDungeoneerEdMode::EM_DungeoneerEdModeId = TEXT("EM_DungeoneerEdMode");
IMPLEMENT_HIT_PROXY(HDungeonTileProxy, HHitProxy);
IMPLEMENT_HIT_PROXY(HDungeonTileWallProxy, HHitProxy);

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
	{
		LevelDungeon = World->SpawnActor<ADungeon>(ADungeon::StaticClass());
	}
	SelectedTiles.Empty();
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
	Canvas->DrawShadowedText(100, 200, FText::FromString("fuck you"), GEngine->GetLargeFont(), FLinearColor::White);
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
				FBox FloorBox = FBox(
					FVector(
						LevelDungeon->DungeonScale * -0.5f,
						LevelDungeon->DungeonScale * -0.5f,
						1.0f) + WorldPosition,
					FVector(
						LevelDungeon->DungeonScale * 0.5f,
						LevelDungeon->DungeonScale * 0.5f,
						0) + WorldPosition);
				
				bool Selected = SelectedTiles.Contains(TilePoints[t]);
				PDI->SetHitProxy(new HDungeonTileProxy(LevelDungeon, TilePoints[t]));
				DrawBox(PDI, FTransform(
					FloorBox.GetCenter()).ToMatrixNoScale(),
					FloorBox.GetExtent(),
					Selected ? LevelDungeon->TileSelectedMaterial->GetRenderProxy() : LevelDungeon->TileUnselectedMaterial->GetRenderProxy(),
					SDPG_Foreground);
				PDI->SetHitProxy(NULL);
				
				for (int i=0; i < DUNGEON_DIRECTIONS.Num(); i++)
				{
					if (LevelDungeon->HasTile(l, TilePoints[t] + DUNGEON_DIRECTIONS[i]))
						continue;
					
					PDI->SetHitProxy(new HDungeonTileWallProxy(LevelDungeon, TilePoints[t], DUNGEON_DIRECTIONS[i]));

					FVector extent = FVector(1.0f, LevelDungeon->DungeonScale * 0.5f, LevelDungeon->DungeonScale * 0.5f);
					if (DUNGEON_DIRECTIONS[i] == EAST_POINT || DUNGEON_DIRECTIONS[i] == WEST_POINT)
						extent = FVector(LevelDungeon->DungeonScale * 0.5f, 1, LevelDungeon->DungeonScale * 0.5f);
					
					FBox NorthWallBox = FBox::BuildAABB(
						WorldPosition + (FVector(DUNGEON_DIRECTIONS[i].X * 0.5f, DUNGEON_DIRECTIONS[i].Y * 0.5f, 0.5f) * LevelDungeon->DungeonScale),
						extent);

					DrawBox(PDI, FTransform(
						NorthWallBox.GetCenter()).ToMatrixNoScale(),
						NorthWallBox.GetExtent(),
						LevelDungeon->TileUnselectedMaterial->GetRenderProxy(),
						SDPG_Foreground);
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
		for (int i=0; i < SelectedTiles.Num(); i++)
			LevelDungeon->DeleteTile(0, SelectedTiles[i]);
		
		SelectedTiles.Empty();
	}
	return FEdMode::InputKey(ViewportClient, Viewport, Key, Event);
}

bool FDungeoneerEdMode::HandleClick(FEditorViewportClient* InViewportClient, HHitProxy* HitProxy,
                                    const FViewportClick& Click)
{
	if (!HitProxy)
	{
		SelectedTiles.Empty();
		return FEdMode::HandleClick(InViewportClient, HitProxy, Click);
	}
	if (HitProxy->IsA(HDungeonTileProxy::StaticGetType()))
	{
		HDungeonTileProxy* TileProxy = (HDungeonTileProxy*)HitProxy;

		if (Click.IsShiftDown())
		{
			if (SelectedTiles.Contains(TileProxy->TilePoint))
				SelectedTiles.Remove(TileProxy->TilePoint);
			else
				SelectedTiles.Emplace(TileProxy->TilePoint);	
		}
		else
		{
			SelectedTiles.Empty();
			SelectedTiles.Emplace(TileProxy->TilePoint);
		}
		return true;
	}
	if (HitProxy->IsA(HDungeonTileWallProxy::StaticGetType()))
	{
		HDungeonTileWallProxy* TileWallProxy = (HDungeonTileWallProxy*)HitProxy;

		FDungeonTile tile;
		LevelDungeon->CreateTile(0, TileWallProxy->TilePoint + TileWallProxy->Direction, tile);
	}

	SelectedTiles.Empty();
	return FEdMode::HandleClick(InViewportClient, HitProxy, Click);
}

bool FDungeoneerEdMode::BoxSelect(FBox& InBox, bool InSelect)
{
	return true;
}




