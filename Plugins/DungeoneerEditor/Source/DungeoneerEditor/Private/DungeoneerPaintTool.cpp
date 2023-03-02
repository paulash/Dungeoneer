#include "DungeoneerPaintTool.h"
#include "DungeoneerTemplateWidget.h"

FDungeoneerPaintTool::FDungeoneerPaintTool(FDungeoneerEditorEdMode* _Mode) : FDungeoneerTool(_Mode)
{
	PanelWidget = SNew(SDungeoneerTemplateWidget);
}
