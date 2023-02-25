#include "DungeoneerSelectTool.h"

void FDungeoneerSelectTool::DrawHUD(FEditorViewportClient* ViewportClient, FViewport* Viewport, const FSceneView* View,
	FCanvas* Canvas)
{
	Canvas->DrawShadowedText(10, 130, FText::FromString("Select Tool Active!"), GEditor->EditorFont, FLinearColor::Red);
}
