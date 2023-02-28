#include "DungeoneerPaintTool.h"

#include "DungeoneerTemplateWidget.h"

TSharedPtr<SCompoundWidget> FDungeoneerPaintTool::GenerateToolPanel()
{
	return SNew(SDungeoneerTemplateWidget);
}
