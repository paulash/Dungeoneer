#include "DungeoneerPaintWidget.h"

void SDungeoneerPaintWidget::Construct(const FArguments& InArgs)
{
	ChildSlot[SNew(STextBlock).Text(FText::FromString("Select Templates used for new tiles."))];
}
