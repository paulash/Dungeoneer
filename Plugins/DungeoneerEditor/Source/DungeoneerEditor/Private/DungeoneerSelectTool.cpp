#include "DungeoneerSelectTool.h"
#include "Dungeon.h"
#include "DungeoneerEditorEdMode.h"
#include "DungeoneerSelectWidget.h"
#include "Materials/MaterialInstanceDynamic.h"

FDungeoneerSelectTool::FDungeoneerSelectTool(FDungeoneerEditorEdMode* _Mode) : FDungeoneerTool(_Mode)
{
	PanelWidget = SNew(SDungeoneerSelectWidget);
}

void FDungeoneerSelectTool::Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI)
{
	FDungeoneerTool::Render(View, Viewport, PDI);

	TArray<FIntVector> TilePoints;
	Mode->LevelDungeon->Tiles.GetKeys(TilePoints);
	for (int i=0; i < TilePoints.Num(); i++)
	{
		FDungeonTile Tile = Mode->LevelDungeon->Tiles[TilePoints[i]];
		for (int s=0; s < DUNGEON_SEGMENT_COUNT; s++)
		{
			if (Mode->LevelDungeon->Tiles.Contains(TilePoints[i] + DUNGEON_DIRECTIONS[s]))
				continue;
			
			FVector WorldPosition =
				Mode->LevelDungeon->GetActorLocation() +
				FVector(
					TilePoints[i].X * Mode->LevelDungeon->Scale,
					TilePoints[i].Y * Mode->LevelDungeon->Scale,
					(TilePoints[i].Z * Mode->LevelDungeon->Scale) + (Mode->LevelDungeon->Scale * 0.5));
			
			FVector center = FVector(
			((DUNGEON_DIRECTIONS[s].X * 0.5f) * Mode->LevelDungeon->Scale) - DUNGEON_DIRECTIONS[s].X,
			((DUNGEON_DIRECTIONS[s].Y * 0.5f) * Mode->LevelDungeon->Scale) - DUNGEON_DIRECTIONS[s].Y,
			((DUNGEON_DIRECTIONS[s].Z * 0.5f) * Mode->LevelDungeon->Scale) - DUNGEON_DIRECTIONS[s].Z);
			center -= FVector(
				DUNGEON_DIRECTIONS[s].X,
				DUNGEON_DIRECTIONS[s].Y,
				DUNGEON_DIRECTIONS[s].Z);

			FVector4 SegmentPoint = FVector4(
				TilePoints[i].X,
				TilePoints[i].Y,
				TilePoints[i].Z,
				s);

			bool selected = SelectedSegments.Contains(SegmentPoint);
			bool hovered = false;
			
			PDI->SetHitProxy(new HDungeonSegmentProxy(TilePoints[i], (EDungeonDirection)s));
			FMaterialRenderProxy* ProxyMaterial = selected ?
					Mode->LevelDungeon->TileSelectedMaterial->GetRenderProxy() :
					Mode->LevelDungeon->TileUnselectedMaterial->GetRenderProxy();;
			if (hovered)
			{
				ProxyMaterial = selected ?
					Mode->LevelDungeon->TileHoveredSelectedMaterial->GetRenderProxy() :
					Mode->LevelDungeon->TileHoveredUnselectedMaterial->GetRenderProxy();	
			}

			DrawPlane10x10(PDI,
				FTransform(
					DUNGEON_SEGMENT_ROTATIONS[s],
					center + WorldPosition).ToMatrixNoScale(),
				(Mode->LevelDungeon->Scale * 0.5f) - 1,
				FVector2D(0, 0),
				FVector2D(1, 1),
				ProxyMaterial,
				SDPG_World);
			PDI->SetHitProxy(NULL);
		}
	}
}

bool FDungeoneerSelectTool::HandleClick(FEditorViewportClient* InViewportClient, HHitProxy* HitProxy,
	const FViewportClick& Click)
{
	
	if (!HitProxy)
	{
		SelectedSegments.Empty();
		return FDungeoneerTool::HandleClick(InViewportClient, HitProxy, Click);
	}
	if (HitProxy->IsA(HDungeonSegmentProxy::StaticGetType()))
	{
		HDungeonSegmentProxy* SegmentProxy = (HDungeonSegmentProxy*)HitProxy;
		if (Click.IsControlDown())
		{
			Mode->LevelDungeon->CreateTile(
				SegmentProxy->TilePoint + DUNGEON_DIRECTIONS[(int)SegmentProxy->Segment]);
		}
		else
		{
			FVector4 Selection = FVector4(
				SegmentProxy->TilePoint.X,
				SegmentProxy->TilePoint.Y,
				SegmentProxy->TilePoint.Z,
				(int)SegmentProxy->Segment);

			if (SelectedSegments.Contains(Selection) && Click.IsShiftDown())
			{
				SelectedSegments.Remove(Selection);
				return true;
			}
			
			if (!Click.IsShiftDown())
				SelectedSegments.Empty();
			
			if (!SelectedSegments.Contains(Selection))
				SelectedSegments.Emplace(Selection);
		}
		return true;
	}
	SelectedSegments.Empty();
	return FDungeoneerTool::HandleClick(InViewportClient, HitProxy, Click);
}