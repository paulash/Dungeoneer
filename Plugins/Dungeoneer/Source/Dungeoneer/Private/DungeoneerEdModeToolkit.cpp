// Copyright Epic Games, Inc. All Rights Reserved.

#include "DungeoneerEdModeToolkit.h"
#include "DungeoneerEdMode.h"
#include "Engine/Selection.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "EditorModeManager.h"
#include "Widgets/Input/STextComboBox.h"
#include "Dungeon.h"
#include "IStructureDetailsView.h"

#define LOCTEXT_NAMESPACE "FDungeoneerEdModeToolkit"

FDungeoneerEdModeToolkit::FDungeoneerEdModeToolkit()
{
}

void FDungeoneerEdModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost)
{
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	
	FStructOnScope* ScopeDungeonPalette = new FStructOnScope(FDungeonPalette::StaticStruct(), (uint8*)&LevelDungeon->DungeonPalette);
	PaletteDetails = PropertyEditorModule.CreateStructureDetailView(
		FDetailsViewArgs(),
		FStructureDetailsViewArgs(),
		MakeShareable(ScopeDungeonPalette),
		FText::FromString("Dungeon Palette")
	);

	//TemplateNames.Add(MakeShareable(new FString("Test1")));
	//TemplateNames.Add(MakeShareable(new FString("Test2")));

	for (int i=0; i < LevelDungeon->DungeonPalette.SegmentTemplates.Num(); i++)
	{
		//TemplateNames.Add(MakeShareable(new FString(LevelDungeon->DungeonPalette.SegmentTemplates[i])));
	}
	
	SAssignNew(ToolkitWidget, SBorder)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(STextComboBox)
				.OptionsSource(&TemplateNames)
				.InitiallySelectedItem(TemplateNames[0])
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				PaletteDetails->GetWidget().ToSharedRef()
			]
	
		];
		
	FModeToolkit::Init(InitToolkitHost);
}

FName FDungeoneerEdModeToolkit::GetToolkitFName() const
{
	return FName("DungeoneerEdMode");
}

FText FDungeoneerEdModeToolkit::GetBaseToolkitName() const
{
	return NSLOCTEXT("DungeoneerEdModeToolkit", "DisplayName", "DungeoneerEdMode Tool");
}

class FEdMode* FDungeoneerEdModeToolkit::GetEditorMode() const
{
	return GLevelEditorModeTools().GetActiveMode(FDungeoneerEdMode::EM_DungeoneerEdModeId);
}

#undef LOCTEXT_NAMESPACE
