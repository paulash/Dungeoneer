#include "DungeoneerTileEditWidget.h"
#include "DungeoneerEditor.h"
#include "DungeoneerEditorEdMode.h"
#include "Widgets/Layout/SScrollBox.h"
#include "EditorModeManager.h"

void SDungeoneerTileEditWidget::Construct(const FArguments& InArgs)
{
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");

	FDetailsViewArgs ViewArgs;
	ViewArgs.bCustomNameAreaLocation = false;
	ViewArgs.NameAreaSettings = FDetailsViewArgs::ENameAreaSettings::HideNameArea;
	ViewArgs.bUpdatesFromSelection = true;
	
	FStructOnScope* ScopeDungeonPalette = new FStructOnScope(FDungeonTile::StaticStruct(), NULL);//(uint8*)&LevelDungeon->DungeonPalette);
	SelectionDetails = PropertyEditorModule.CreateStructureDetailView(
		FDetailsViewArgs(),
		FStructureDetailsViewArgs(),
		MakeShareable(ScopeDungeonPalette),
		FText::FromString("Dungeon Model")
	);
	SelectionDetails->GetOnFinishedChangingPropertiesDelegate().AddRaw(this, &SDungeoneerTileEditWidget::OnFinishDetails);

	TileDetails = SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.VAlign(VAlign_Top)
			.AutoHeight()
			[
				SelectionDetails->GetWidget().ToSharedRef()
			]
			/*
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
			]*/;
	ChildSlot
	[
		TileDetails.ToSharedRef()
	];

	TileDetails->SetVisibility(EVisibility::Collapsed);
}

void SDungeoneerTileEditWidget::OnUpdateSelection(TArray<FVector4> SelectedTiles)
{
	TileDetails->SetVisibility(EVisibility::Collapsed);
	if (SelectedTiles.Num() == 1)
	{
		FIntVector TilePoint = FIntVector(SelectedTiles[0].X, SelectedTiles[0].Y, SelectedTiles[0].Z);
		/*
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
		}
		*/

		FStructOnScope* ScopeTileTags = new FStructOnScope(FDungeonTile::StaticStruct(),
			(uint8*)&FDungeoneerEditorEdMode::GetEdMode()->LevelDungeon->Tiles[TilePoint]);
		SelectionDetails->SetStructureData(MakeShareable(ScopeTileTags));
		SelectionDetails->SetCustomName(FText::FromString(
			FString::FromInt(TilePoint.X) + ", " +
			FString::FromInt(TilePoint.Y) + ", " +
 			FString::FromInt(TilePoint.Z)));

		TileDetails->SetVisibility(EVisibility::Visible);
	}
}

void SDungeoneerTileEditWidget::Shutdown()
{
	SelectionDetails = NULL;
}

void SDungeoneerTileEditWidget::OnFinishDetails(const FPropertyChangedEvent& evt)
{
	FDungeoneerEditorEdMode::GetEdMode()->LevelDungeon->Modify();
	FDungeoneerEditorEdMode::GetEdMode()->LevelDungeon->RegenerateTiles();
}
