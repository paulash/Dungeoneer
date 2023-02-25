#include "DungeoneerSelectTool.h"
#include "Dungeon.h"
#include "DungeoneerEditorEdMode.h"
#include "Materials/MaterialInstanceDynamic.h"

void FDungeoneerSelectTool::DrawHUD(FEditorViewportClient* ViewportClient, FViewport* Viewport, const FSceneView* View,
	FCanvas* Canvas)
{
	Canvas->DrawShadowedText(10, 130, FText::FromString("Select Tool Active!"), GEditor->EditorFont, FLinearColor::Red);
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

			FIntVector4 SegmentPoint = FIntVector4(
				TilePoints[i].X,
				TilePoints[i].Y,
				TilePoints[i].Z,
				s);

			bool selected = false;//SelectedSegments.Contains(SegmentPoint);
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
		}
	}
}
