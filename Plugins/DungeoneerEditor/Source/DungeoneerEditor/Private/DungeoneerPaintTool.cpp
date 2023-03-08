#include "DungeoneerPaintTool.h"
#include "DungeoneerPaintWidget.h"
#include "DungeoneerEditorEdMode.h"

FDungeoneerPaintTool::FDungeoneerPaintTool(FDungeoneerEditorEdMode* _Mode) : FDungeoneerTool(_Mode)
{
	PanelWidget = SNew(SDungeoneerPaintWidget);
}

void FDungeoneerPaintTool::Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI)
{
	FDungeoneerTool::Render(View, Viewport, PDI);

	if (ValidHover)
	{
		FVector WorldPosition =
			Mode->LevelDungeon->GetActorLocation() +
			FVector(
				HoveredSegment.X * Mode->LevelDungeon->Scale,
				HoveredSegment.Y * Mode->LevelDungeon->Scale,
				(HoveredSegment.Z * Mode->LevelDungeon->Scale) + (Mode->LevelDungeon->Scale * 0.5));
		
		FVector center = FVector(
		((DUNGEON_SEGMENT_OFFSETS[HoveredSegment.W].X * 0.5f) * Mode->LevelDungeon->Scale),
		((DUNGEON_SEGMENT_OFFSETS[HoveredSegment.W].Y * 0.5f) * Mode->LevelDungeon->Scale), 
		((DUNGEON_SEGMENT_OFFSETS[HoveredSegment.W].Z * 0.5f) * Mode->LevelDungeon->Scale));
		center -= FVector(
			DUNGEON_SEGMENT_OFFSETS[HoveredSegment.W].X,
			DUNGEON_SEGMENT_OFFSETS[HoveredSegment.W].Y,
			DUNGEON_SEGMENT_OFFSETS[HoveredSegment.W].Z);

		FVector size = FVector(FMath::Abs(DUNGEON_SEGMENT_OFFSETS[HoveredSegment.W].X) == 1 ? 0 : 1,
								FMath::Abs(DUNGEON_SEGMENT_OFFSETS[HoveredSegment.W].Y) == 1 ? 0 : 1,
								FMath::Abs(DUNGEON_SEGMENT_OFFSETS[HoveredSegment.W].Z) == 1 ? 0 : 1);
		FBox box = FBox::BuildAABB(center + WorldPosition, FVector(size * (Mode->LevelDungeon->Scale/2) - 1));
		
		if (FDungeoneerEditorEdMode::GetEdMode()->IsCtrlDown())
		{
			DrawWireBox(PDI, box, FColor::FromHex("99CCFF"), SDPG_World, 4);
		}
		else if (FDungeoneerEditorEdMode::GetEdMode()->IsShiftDown())
		{
			DrawWireBox(PDI, FBox::BuildAABB(
				WorldPosition,
				FVector(Mode->LevelDungeon->Scale/2)),
				FLinearColor::Red,
				SDPG_World,
				4);
		}
		else
		{
			DrawWireBox(PDI, FBox::BuildAABB(
				WorldPosition,
				FVector(Mode->LevelDungeon->Scale/2)),
				FLinearColor::Gray,
				SDPG_World,
				1);
			DrawWireBox(PDI, box, FLinearColor::White, SDPG_World, 4);
		}
	}
}

bool FDungeoneerPaintTool::HandleClick(FEditorViewportClient* InViewportClient, HHitProxy* HitProxy,
                                       const FViewportClick& Click)
{
	if (HitProxy && HitProxy->IsA(HInstancedStaticMeshInstance::StaticGetType()))
	{
		HInstancedStaticMeshInstance* ISMI = (HInstancedStaticMeshInstance*)HitProxy;
		if (ISMI)
		{
			FIntVector TilePoint;
			EDungeonSegment Direction = EDungeonSegment::NORTH;
			int CustomModelIndex = -1;
			if (FDungeoneerEditorEdMode::GetEdMode()->LevelDungeon->GetHitInfo(ISMI, TilePoint, Direction, CustomModelIndex))
			{
				if (Click.GetKey() == EKeys::RightMouseButton)
				{
					DUNGEON_SEGMENT_ROTATIONS[(int)Direction];
					//FDungeoneerEditorEdMode::GetEdMode()->RotateTile(
					//	TilePoint, Direction, );
				}
				if (Click.GetKey() == EKeys::LeftMouseButton)
				{
					if (Click.IsControlDown())
						FDungeoneerEditorEdMode::GetEdMode()->AddTile(TilePoint + DUNGEON_SEGMENT_OFFSETS[(int)Direction]);	
					else if (Click.IsShiftDown())
						FDungeoneerEditorEdMode::GetEdMode()->RemoveTile(TilePoint);
					else if (FDungeoneerEditorEdMode::GetEdMode()->GetSelectedTemplate() != NAME_None)
						FDungeoneerEditorEdMode::GetEdMode()->UpdateTile(
							TilePoint, Direction, FDungeoneerEditorEdMode::GetEdMode()->GetSelectedTemplate());
				}
			}
		}
		return true;
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
			EDungeonSegment Direction = EDungeonSegment::NORTH;
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

void FDungeoneerPaintTool::DrawHUD(FEditorViewportClient* ViewportClient, FViewport* Viewport, const FSceneView* View,
	FCanvas* Canvas)
{
	if (!ValidHover) return;
	FString HoverInfo = "";
	HoverInfo += FString::FromInt((int)HoveredSegment.X);
	HoverInfo += ", ";
	HoverInfo += FString::FromInt((int)HoveredSegment.Y);
	HoverInfo += ", ";
	HoverInfo += FString::FromInt((int)HoveredSegment.Z);
	HoverInfo += " [";
	HoverInfo += FString::FromInt((int)HoveredSegment.W);
	HoverInfo += "]";
	
	Canvas->DrawShadowedText(50, 50, FText::FromString(HoverInfo), GEditor->GetMediumFont(), FLinearColor::White);
}
