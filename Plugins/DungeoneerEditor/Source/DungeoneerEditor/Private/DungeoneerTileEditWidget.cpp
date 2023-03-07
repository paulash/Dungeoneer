#include "DungeoneerTileEditWidget.h"
#include "DungeoneerEditor.h"
#include "DungeoneerEditorEdMode.h"
#include "Widgets/Layout/SScrollBox.h"
#include "EditorModeManager.h"

void SDungeoneerTileEditWidget::Construct(const FArguments& InArgs)
{
	NorthCombo = SNew(SDungeoneerTemplateCombo);
	EastCombo = SNew(SDungeoneerTemplateCombo);
	SouthCombo = SNew(SDungeoneerTemplateCombo);
	WestCombo = SNew(SDungeoneerTemplateCombo);
	UpCombo = SNew(SDungeoneerTemplateCombo);
	DownCombo = SNew(SDungeoneerTemplateCombo);
	ModelCombo = SNew(SDungeoneerTemplateCombo);

	NorthCombo->OnSelectionChanged()
	
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
						NorthCombo.ToSharedRef()
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
						EastCombo.ToSharedRef()
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
						SouthCombo.ToSharedRef()
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
						WestCombo.ToSharedRef()
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
						UpCombo.ToSharedRef()
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
						DownCombo.ToSharedRef()
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
						SNew(STextBlock).Text(FText::FromString("MODEL"))
					]
					+ SHorizontalBox::Slot()
					.HAlign(HAlign_Fill)
					.FillWidth(1.0)
					[
						ModelCombo.ToSharedRef()
					]
			]
	];
}

void SDungeoneerTileEditWidget::OnUpdateSelection(TArray<FVector4> SelectedTiles)
{
	if (SelectedTiles.Num() == 1)
	{
		FIntVector TilePoint = FIntVector(SelectedTiles[0].X, SelectedTiles[0].Y, SelectedTiles[0].Z);
		EDungeonSegment Segment = (EDungeonSegment)SelectedTiles[0].W;

		FDungeonTile Tile;
		if (FDungeoneerEditorEdMode::GetEdMode()->LevelDungeon->GetTile(TilePoint, Tile))
		{
			NorthCombo->SetSelection(Tile.SegmentModels[0]);
			EastCombo->SetSelection(Tile.SegmentModels[1]);
			SouthCombo->SetSelection(Tile.SegmentModels[2]);
			WestCombo->SetSelection(Tile.SegmentModels[3]);
			UpCombo->SetSelection(Tile.SegmentModels[4]);
			DownCombo->SetSelection(Tile.SegmentModels[5]);
			ModelCombo->SetSelection(Tile.SegmentModels[6]);
		}
	}
}

void SDungeoneerTileEditWidget::OnFinishDetails(const FPropertyChangedEvent& evt)
{
	FDungeoneerEditorEdMode::GetEdMode()->LevelDungeon->RegenerateTiles();
}
