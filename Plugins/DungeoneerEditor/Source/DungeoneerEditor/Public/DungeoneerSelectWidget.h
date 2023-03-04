#pragma once
#include "DungeoneerEditorEdMode.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Views/SListView.h"
#include "IStructureDetailsView.h"

class SDungeoneerSelectWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDungeoneerSelectWidget) {}
	SLATE_END_ARGS();

	void Construct(const FArguments& InArgs);

private:
	
	TSharedPtr<IStructureDetailsView> SelectionDetails;
};