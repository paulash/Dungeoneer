#pragma once
#include "DungeoneerEditorEdMode.h"
#include "Widgets/SCompoundWidget.h"
#include "DungeoneerTemplateCombo.h"
#include "IStructureDetailsView.h"

class SDungeoneerTileEditWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDungeoneerTileEditWidget) {}
	SLATE_END_ARGS();

	void Construct(const FArguments& InArgs);
	void OnUpdateSelection(TArray<FVector4> SelectedTiles);
	
private:

	void OnFinishDetails(const FPropertyChangedEvent& evt);
	
	TSharedPtr<STextBlock> TestText;
	
	TSharedPtr<IStructureDetailsView> SelectionDetails;

	TSharedPtr<SDungeoneerTemplateCombo> NorthCombo;
	TSharedPtr<SDungeoneerTemplateCombo> EastCombo;
	TSharedPtr<SDungeoneerTemplateCombo> SouthCombo;
	TSharedPtr<SDungeoneerTemplateCombo> WestCombo;
	TSharedPtr<SDungeoneerTemplateCombo> UpCombo;
	TSharedPtr<SDungeoneerTemplateCombo> DownCombo;
	TSharedPtr<SDungeoneerTemplateCombo> ModelCombo;
};