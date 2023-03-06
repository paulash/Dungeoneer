#include "DungeoneerPaintTool.h"
#include "DungeoneerTemplateWidget.h"

FDungeoneerPaintTool::FDungeoneerPaintTool(FDungeoneerEditorEdMode* _Mode) : FDungeoneerTool(_Mode)
{
	PanelWidget = SNew(SDungeoneerTemplateWidget);
}

void FDungeoneerPaintTool::Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI)
{
	FDungeoneerTool::Render(View, Viewport, PDI);

	if (ValidHover)
	{
		//TODO: render a marker for ctrl/shift mode as well as normal paint mode.
		//TODO: for normal paint, just draw a selection box like in the edit mode.
	}
}

bool FDungeoneerPaintTool::HandleClick(FEditorViewportClient* InViewportClient, HHitProxy* HitProxy,
                                       const FViewportClick& Click)
{
	if (HitProxy && HitProxy->IsA(HInstancedStaticMeshInstance::StaticGetType()) && Click.IsControlDown())
	{
		HInstancedStaticMeshInstance* ISMI = (HInstancedStaticMeshInstance*)HitProxy;
		if (ISMI)
		{
			FIntVector TilePoint;
			EDungeonDirection Direction = EDungeonDirection::NORTH;
			int CustomModelIndex = -1;
			if (FDungeoneerEditorEdMode::GetEdMode()->LevelDungeon->GetHitInfo(ISMI, TilePoint, Direction, CustomModelIndex))
			{
				FDungeoneerEditorEdMode::GetEdMode()->AddTile(TilePoint + DUNGEON_DIRECTIONS[(int)Direction]);
			}
		}
	}
	return false;
}

bool FDungeoneerPaintTool::MouseMove(FEditorViewportClient* ViewportClient, FViewport* Viewport, int32 x, int32 y)
{
	ValidHover = false;
	
	HHitProxy* HitProxy = Viewport->GetHitProxy(x, y);
	if (HitProxy && HitProxy->IsA(HInstancedStaticMeshInstance::StaticGetType()))
	{
		HInstancedStaticMeshInstance* ISMI = (HInstancedStaticMeshInstance*)HitProxy;
		if (ISMI)
		{
			FIntVector TilePoint;
			EDungeonDirection Direction = EDungeonDirection::NORTH;
			int CustomModelIndex = -1;
			if (FDungeoneerEditorEdMode::GetEdMode()->LevelDungeon->GetHitInfo(ISMI, TilePoint, Direction, CustomModelIndex))
			{
				ValidHover = true;
				HoveredSegment = FVector4(TilePoint.X, TilePoint.Y, TilePoint.Z, (int)Direction);
			}
		}
	}
	
	return false;
}
