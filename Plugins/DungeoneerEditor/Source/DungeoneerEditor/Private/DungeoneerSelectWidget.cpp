#include "DungeoneerSelectWidget.h"
#include "DungeoneerEditor.h"
#include "DungeoneerEditorEdMode.h"
#include "Widgets/Layout/SScrollBox.h"
#include "EditorModeManager.h"

void SDungeoneerSelectWidget::Construct(const FArguments& InArgs)
{
	/*
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");

	FDetailsViewArgs args;
	args.bAllowSearch = false;
	args.bShowActorLabel = false;
	args.bAllowMultipleTopLevelObjects = false;

	FIntVector tilePoint = FIntVector(0,0,0);
	FStructOnScope* ScopeDungeonPalette = new FStructOnScope(FDungeonTile::StaticStruct(), NULL);
	SelectionDetails = PropertyEditorModule.CreateStructureDetailView(
		args,
		FStructureDetailsViewArgs(),
		MakeShareable(ScopeDungeonPalette),
		FText::FromString("Dungeon Tile")
	);
	*/

	/*
	(uint8*)&GetEdMode()->LevelDungeon->Tiles[tilePoint]
	FStructOnScope* ScopeDungeonPalette = new FStructOnScope(FDungeonModel::StaticStruct(),
		(uint8*)&GetEdMode()->LevelDungeon->DungeonPalette.Models[GetEdMode()->SelectedTemplate]);
	SelectionDetails->SetStructureData(MakeShareable(ScopeDungeonPalette));
	*/

	SNew(SComboBox<TSharedPtr<FString>>);
	
	ChildSlot
	[
		SNew(STextBlock).Text(FText::FromString("woot select."))//SelectionDetails->GetWidget().ToSharedRef()
	];
}
