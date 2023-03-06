#include "DungeoneerTileEditWidget.h"
#include "DungeoneerEditor.h"
#include "DungeoneerEditorEdMode.h"
#include "DungeoneerTemplateCombo.h"
#include "Widgets/Layout/SScrollBox.h"
#include "EditorModeManager.h"

void SDungeoneerTileEditWidget::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.VAlign(VAlign_Top)
			.AutoHeight()
			[
				SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.HAlign(HAlign_Fill)
					.FillWidth(0.33)
					[
						SNew(STextBlock).Text(FText::FromString("NORTH"))
					]
					+ SHorizontalBox::Slot()
					.HAlign(HAlign_Fill)
					.FillWidth(1.0)
					[
						SNew(SDungeoneerTemplateCombo)
					]
			]
			+ SVerticalBox::Slot()
			.VAlign(VAlign_Top)
			.AutoHeight()
			[
				SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.HAlign(HAlign_Fill)
					.FillWidth(0.33)
					[
						SNew(STextBlock).Text(FText::FromString("EAST"))
					]
					+ SHorizontalBox::Slot()
					.HAlign(HAlign_Fill)
					.FillWidth(1.0)
					[
						SNew(SDungeoneerTemplateCombo)
					]
			]
			+ SVerticalBox::Slot()
			.VAlign(VAlign_Top)
			.AutoHeight()
			[

				SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.HAlign(HAlign_Fill)
					.FillWidth(0.33)
					[
						SNew(STextBlock).Text(FText::FromString("SOUTH"))
					]
					+ SHorizontalBox::Slot()
					.HAlign(HAlign_Fill)
					.FillWidth(1.0)
					[
						SNew(SDungeoneerTemplateCombo)
					]
			]
			+ SVerticalBox::Slot()
			.VAlign(VAlign_Top)
			.AutoHeight()
			[
				SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.HAlign(HAlign_Fill)
					.FillWidth(0.33)
					[
						SNew(STextBlock).Text(FText::FromString("WEST"))
					]
					+ SHorizontalBox::Slot()
					.HAlign(HAlign_Fill)
					.FillWidth(1.0)
					[
						SNew(SDungeoneerTemplateCombo)
					]
			]
			+ SVerticalBox::Slot()
			.VAlign(VAlign_Top)
			.AutoHeight()
			[
				SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.HAlign(HAlign_Fill)
					.FillWidth(0.33)
					[
						SNew(STextBlock).Text(FText::FromString("UP"))
					]
					+ SHorizontalBox::Slot()
					.HAlign(HAlign_Fill)
					.FillWidth(1.0)
					[
						SNew(SDungeoneerTemplateCombo)
					]
			]
			+ SVerticalBox::Slot()
			.VAlign(VAlign_Top)
			.AutoHeight()
			[
				SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.HAlign(HAlign_Fill)
					.FillWidth(0.33)
					[
						SNew(STextBlock).Text(FText::FromString("DOWN"))
					]
					+ SHorizontalBox::Slot()
					.HAlign(HAlign_Fill)
					.FillWidth(1.0)
					[
						SNew(SDungeoneerTemplateCombo)
					]
			]
	];
}

void SDungeoneerTileEditWidget::OnUpdateSelection(TArray<FVector4> SelectedTiles)
{
}

void SDungeoneerTileEditWidget::OnFinishDetails(const FPropertyChangedEvent& evt)
{
	FDungeoneerEditorEdMode::GetEdMode()->LevelDungeon->RegenerateTiles();
}
