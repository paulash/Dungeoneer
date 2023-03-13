#include "DungeoneerTileEditTool.h"

#include "DrawDebugHelpers.h"
#include "Dungeon.h"
#include "DungeoneerEditorEdMode.h"
#include "DungeoneerTileEditWidget.h"
#include "Materials/MaterialInstanceDynamic.h"

FDungeoneerTileEditTool::FDungeoneerTileEditTool(FDungeoneerEditorEdMode* _Mode) : FDungeoneerTool(_Mode)
{
	PanelWidget = SNew(SDungeoneerTileEditWidget);
}

void FDungeoneerTileEditTool::Shutdown()
{
	((SDungeoneerTileEditWidget*)PanelWidget.Get())->Shutdown();
	FDungeoneerTool::Shutdown();
}

void FDungeoneerTileEditTool::Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI)
{
	FDungeoneerTool::Render(View, Viewport, PDI);

	TArray<FIntVector> TilePoints;
	Mode->LevelDungeon->Tiles.GetKeys(TilePoints);
	for (int i=0; i < TilePoints.Num(); i++)
	{
		FDungeonTile Tile = Mode->LevelDungeon->Tiles[TilePoints[i]];
		bool drawTile = false;
		
		for (int s=0; s < DUNGEON_SEGMENT_COUNT; s++)
		{
			if (Mode->LevelDungeon->Tiles.Contains(TilePoints[i] + DUNGEON_SEGMENT_OFFSETS[s]))
				continue;
			
			FVector WorldPosition =
				Mode->LevelDungeon->GetActorLocation() +
				FVector(
					TilePoints[i].X * Mode->LevelDungeon->Scale,
					TilePoints[i].Y * Mode->LevelDungeon->Scale,
					(TilePoints[i].Z * Mode->LevelDungeon->Scale) + (Mode->LevelDungeon->Scale * 0.5));
			
			FVector center = FVector(
			((DUNGEON_SEGMENT_OFFSETS[s].X * 0.5f) * Mode->LevelDungeon->Scale),
			((DUNGEON_SEGMENT_OFFSETS[s].Y * 0.5f) * Mode->LevelDungeon->Scale), 
			((DUNGEON_SEGMENT_OFFSETS[s].Z * 0.5f) * Mode->LevelDungeon->Scale));
			center -= FVector(
				DUNGEON_SEGMENT_OFFSETS[s].X,
				DUNGEON_SEGMENT_OFFSETS[s].Y,
				DUNGEON_SEGMENT_OFFSETS[s].Z);

			FVector size = FVector(FMath::Abs(DUNGEON_SEGMENT_OFFSETS[s].X) == 1 ? 0 : 1,
									FMath::Abs(DUNGEON_SEGMENT_OFFSETS[s].Y) == 1 ? 0 : 1,
									FMath::Abs(DUNGEON_SEGMENT_OFFSETS[s].Z) == 1 ? 0 : 1);
			FBox box = FBox::BuildAABB(center + WorldPosition, FVector(size * (Mode->LevelDungeon->Scale/2) - 1));
			
			FVector4 SegmentPoint = FVector4(
				TilePoints[i].X,
				TilePoints[i].Y,
				TilePoints[i].Z,
				s);

			
			bool selected = SelectedSegments.Contains(SegmentPoint);
			bool hovered = HoveredSegmentValid ? SegmentPoint == HoveredSegment : false;
			
			if (selected || hovered)
			{
				if (!drawTile)
				{
					DrawWireBox(PDI, FBox::BuildAABB(
						WorldPosition,
						FVector(Mode->LevelDungeon->Scale/2)),
						FLinearColor(0.8f, 0.8f, 0.8f, 0.8f),
						SDPG_World,
						hovered? 4 : 2);
					
					drawTile = true;
				}
				//DrawWireBox(PDI, box, FLinearColor::Yellow, SDPG_World, hovered? 4 : 1);
			}
		}
	}
}

bool FDungeoneerTileEditTool::HandleClick(FEditorViewportClient* InViewportClient, HHitProxy* HitProxy,
	const FViewportClick& Click)
{
	if (!HitProxy)
	{
		SelectedSegments.Empty();
		((SDungeoneerTileEditWidget*)PanelWidget.Get())->OnUpdateSelection(SelectedSegments);
		return FDungeoneerTool::HandleClick(InViewportClient, HitProxy, Click);
	}
	if (HitProxy->IsA(HInstancedStaticMeshInstance::StaticGetType()))
	{
		HInstancedStaticMeshInstance* HISMI = (HInstancedStaticMeshInstance*)HitProxy;
		
		FIntVector TilePoint;
		EDungeonSegment Direction;
		int CustomModelIndex;
		
		if (FDungeoneerEditorEdMode::GetEdMode()->LevelDungeon->GetHitInfo(HISMI, TilePoint, Direction, CustomModelIndex))
		{
			FVector4 Selection = FVector4(
				TilePoint.X,
				TilePoint.Y,
				TilePoint.Z,
				(int)Direction);
			// later we'll add multi-selection to edit values in a batch of tiles, for now we disable this.
			/*
			if (SelectedSegments.Contains(Selection) && Click.IsShiftDown())
			{
				SelectedSegments.Remove(Selection);
				return true;
			}
			
			if (!Click.IsShiftDown())
				SelectedSegments.Empty();
			
			if (!SelectedSegments.Contains(Selection))
				SelectedSegments.Emplace(Selection);
			*/
			SelectedSegments.Empty();
			SelectedSegments.Emplace(Selection);
			((SDungeoneerTileEditWidget*)PanelWidget.Get())->OnUpdateSelection(SelectedSegments);
			return true;
		}
	}
	
	SelectedSegments.Empty();
	((SDungeoneerTileEditWidget*)PanelWidget.Get())->OnUpdateSelection(SelectedSegments);
	return FDungeoneerTool::HandleClick(InViewportClient, HitProxy, Click);
}

bool FDungeoneerTileEditTool::MouseMove(FEditorViewportClient* ViewportClient, FViewport* Viewport, int32 x, int32 y)
{
	HoveredSegmentValid = false;
	HHitProxy* hitProxy = Viewport->GetHitProxy(x, y);
	if (hitProxy && hitProxy->IsA(HInstancedStaticMeshInstance::StaticGetType()))
	{
		HInstancedStaticMeshInstance* HISMI = (HInstancedStaticMeshInstance*)hitProxy;
		if (HISMI)
		{
			FIntVector TilePoint;
			EDungeonSegment Direction;
			int CustomModelIndex;
		
			if (FDungeoneerEditorEdMode::GetEdMode()->LevelDungeon->GetHitInfo(HISMI, TilePoint, Direction, CustomModelIndex))
			{
				HoveredSegmentValid = true;
				HoveredSegment = FVector4(
					TilePoint.X,
					TilePoint.Y,
					TilePoint.Z,
					(int)Direction);
			}
			return true;	
		}
	}
	return false;
}

bool FDungeoneerTileEditTool::InputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key,
	EInputEvent Event)
{
	if (Key == EKeys::Escape && Event == EInputEvent::IE_Pressed)
	{
		SelectedSegments.Empty();
		((SDungeoneerTileEditWidget*)PanelWidget.Get())->OnUpdateSelection(SelectedSegments);
		return true;
	}
	return false;
}
