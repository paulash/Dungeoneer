#include "DungeoneerTileEditWidget.h"
#include "DungeoneerEditor.h"
#include "DungeoneerEditorEdMode.h"
#include "DungeoneerTemplateCombo.h"
#include "Widgets/Layout/SScrollBox.h"
#include "EditorModeManager.h"

void SDungeoneerTileEditWidget::Construct(const FArguments& InArgs)
{
	/*
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");

	FDetailsViewArgs args;
	args.bAllowSearch = false;
	args.bShowActorLabel = false;
	args.bAllowMultipleTopLevelObjects = false;

	FIntVector tilePoint = FIntVector(0,0,0);
	FStructOnScope* ScopeDungeonPalette = new FStructOnScope(FGameplayTagContainer::StaticStruct(), NULL);
	SelectionDetails = PropertyEditorModule.CreateStructureDetailView(
		args,
		FStructureDetailsViewArgs(),
		MakeShareable(ScopeDungeonPalette),
		FText::FromString("Dungeon Tile Tags")
	);
	SelectionDetails->GetOnFinishedChangingPropertiesDelegate().AddRaw(this, &SDungeoneerTileEditWidget::OnFinishDetails);
	*/

	
	ChildSlot
	[
		SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.VAlign(VAlign_Top)
			.AutoHeight()
			[
				SNew(SDungeoneerTemplateCombo)
			]
			
	];
}

void SDungeoneerTileEditWidget::OnUpdateSelection(TArray<FVector4> SelectedTiles)
{
	/*
	if (SelectedTiles.Num() == 1)
	{
		FStructOnScope* ScopeDungeonPalette = new FStructOnScope(FGameplayTagContainer::StaticStruct(),
			(uint8*)&FDungeoneerEditorEdMode::GetEdMode()->LevelDungeon->Tiles[FIntVector(SelectedTiles[0].X, SelectedTiles[0].Y, SelectedTiles[0].Z)].Tags);
	
		SelectionDetails->SetStructureData(MakeShareable(ScopeDungeonPalette));	
	}
	*/
	
	/*
	FString text = "";
	for (int i=0; i < TileVectors.Num(); i++)
	{
		FVector4 TileVector = TileVectors[i];
		FIntVector TilePoint = FIntVector(TileVector.X, TileVector.Y, TileVector.Z);
		EDungeonDirection Segment = (EDungeonDirection)TileVector.W;
		
		FDungeonTile Tile;
		if (!FDungeoneerEditorEdMode::GetEdMode()->LevelDungeon->GetTile(TilePoint, Tile))
			continue;

		text += Tile.SegmentModels[(int)Segment].ToString() + ",";
	}
	*/
	
	//TestText.Get()->SetText(FText::FromString(text));
}

void SDungeoneerTileEditWidget::OnFinishDetails(const FPropertyChangedEvent& evt)
{
	FDungeoneerEditorEdMode::GetEdMode()->LevelDungeon->RegenerateTiles();
}
