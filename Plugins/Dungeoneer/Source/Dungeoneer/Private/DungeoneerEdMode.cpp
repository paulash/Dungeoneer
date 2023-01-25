#include "DungeoneerEdMode.h"
#include "DungeoneerEdModeToolkit.h"
#include "Toolkits/ToolkitManager.h"
#include "EditorModeManager.h"
#include "EngineUtils.h"

const FEditorModeID FDungeoneerEdMode::EM_DungeoneerEdModeId = TEXT("EM_DungeoneerEdMode");
IMPLEMENT_HIT_PROXY(HDungeonSegmentProxy, HHitProxy);


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

	// initialize editor materials.
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
	}
	SegmentSelections.Empty();
	
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

void FDungeoneerEdMode::Tick(FEditorViewportClient* ViewportClient, float DeltaTime)
{
	FEdMode::Tick(ViewportClient, DeltaTime);
}

void FDungeoneerEdMode::Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI)
{
	if (LevelDungeon)
	{
		TArray<FIntVector> TilePoints;
		LevelDungeon->GetTilePoints(TilePoints);
		for (int t=0; t < TilePoints.Num(); t++)
		{
			FDungeonTile Tile;
			if (LevelDungeon->GetTile(TilePoints[t], Tile))
			{
				FVector WorldPosition =
				LevelDungeon->GetActorLocation() +
				FVector(
					TilePoints[t].X * LevelDungeon->Scale,
					TilePoints[t].Y * LevelDungeon->Scale,
					(TilePoints[t].Z * LevelDungeon->Scale) + (LevelDungeon->Scale * 0.5));
				for (int s=0; s < Tile.Segments.Num(); s++)
				{
					EDungeonTileSegment Segment = (EDungeonTileSegment)s;
					if (LevelDungeon->HasTile(TilePoints[t] + DUNGEON_DIRECTIONS[(int)Segment]))
						continue;
					
					FVector center = FVector(
					((DUNGEON_DIRECTIONS[s].X * 0.5f) * LevelDungeon->Scale) - DUNGEON_DIRECTIONS[s].X,
					((DUNGEON_DIRECTIONS[s].Y * 0.5f) * LevelDungeon->Scale) - DUNGEON_DIRECTIONS[s].Y,
					((DUNGEON_DIRECTIONS[s].Z * 0.5f) * LevelDungeon->Scale) - DUNGEON_DIRECTIONS[s].Z);
					center -= FVector(DUNGEON_DIRECTIONS[s].X, DUNGEON_DIRECTIONS[s].Y, DUNGEON_DIRECTIONS[s].Z) * 1;

					
					FRotator rotation = DUNGEON_SEGMENT_ROTATIONS[s];

					FDungeonSegmentSelection selection = FDungeonSegmentSelection(TilePoints[t], Segment);
					bool selected = SegmentSelections.Contains(selection);
					
					PDI->SetHitProxy(new HDungeonSegmentProxy(TilePoints[t], Segment));
					DrawPlane10x10(PDI,
						FTransform(rotation, center + WorldPosition).ToMatrixNoScale(),
						(LevelDungeon->Scale * 0.5f) - 1,
						FVector2D(0, 0),
						FVector2D(1, 1),
						selected ? LevelDungeon->TileSelectedMaterial->GetRenderProxy() : LevelDungeon->TileUnselectedMaterial->GetRenderProxy(),
						SDPG_World);
					PDI->SetHitProxy(NULL);
				}
			}
		}


		
	}

	
	FEdMode::Render(View, Viewport, PDI);
}

bool FDungeoneerEdMode::InputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key,
	EInputEvent Event)
{
	return FEdMode::InputKey(ViewportClient, Viewport, Key, Event);
}

bool FDungeoneerEdMode::HandleClick(FEditorViewportClient* InViewportClient, HHitProxy* HitProxy,
                                    const FViewportClick& Click)
{
	if (!HitProxy)
		return FEdMode::HandleClick(InViewportClient, HitProxy, Click);


	if (HitProxy->IsA(HDungeonSegmentProxy::StaticGetType()))
	{
		HDungeonSegmentProxy* SegmentProxy = (HDungeonSegmentProxy*)HitProxy;
		if (Click.IsControlDown())
		{
			LevelDungeon->CreateTile(
				SegmentProxy->TilePoint + DUNGEON_DIRECTIONS[(int)SegmentProxy->Segment],
				LevelDungeon->DefaultTileTemplate);
		}
		else
		{
			
			FDungeonSegmentSelection Selection = FDungeonSegmentSelection(SegmentProxy->TilePoint, SegmentProxy->Segment);

			SegmentSelections.Empty();
			SegmentSelections.Emplace(Selection);
		}
		

	}

	
	return FEdMode::HandleClick(InViewportClient, HitProxy, Click);
}

bool FDungeoneerEdMode::UsesToolkits() const
{
	return true;
}




