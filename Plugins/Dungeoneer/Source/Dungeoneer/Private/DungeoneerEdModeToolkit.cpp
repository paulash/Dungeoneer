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
	PaletteDetails->GetOnFinishedChangingPropertiesDelegate().AddRaw(this, &FDungeoneerEdModeToolkit::OnFinishDetails);

	TArray<FName> _TemplateNames;
	LevelDungeon->DungeonPalette.Models.GetKeys(_TemplateNames);
	for (int i=0; i < _TemplateNames.Num(); i++)
	{
		FString name = _TemplateNames[i].ToString();
		FString* sName = new FString(name);
		TemplateNames.Add(MakeShareable(sName));
	}
	if (TemplateNames.Num() == 0)
		TemplateNames.Add(MakeShareable(new FString("NO TEMPLATES!")));

	TemplateComboBox = SNew(STextComboBox)
						.OptionsSource(&TemplateNames)
						.InitiallySelectedItem(TemplateNames[0])
						.OnSelectionChanged(this, &FDungeoneerEdModeToolkit::OnSelectTemplate);
	
	SAssignNew(ToolkitWidget, SBorder)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				TemplateComboBox.ToSharedRef()
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

void FDungeoneerEdModeToolkit::OnFinishDetails(const FPropertyChangedEvent& evt)
{
	TemplateNames.Empty();
	TArray<FName> _TemplateNames;
	LevelDungeon->DungeonPalette.Models.GetKeys(_TemplateNames);
	for (int i=0; i < _TemplateNames.Num(); i++)
	{
		FString name = _TemplateNames[i].ToString();
		FString* sName = new FString(name);
		TemplateNames.Add(MakeShareable(sName));	
	}
	if (TemplateNames.Num() == 0)
		TemplateNames.Add(MakeShareable(new FString("NO TEMPLATES!")));

	TemplateComboBox->SetSelectedItem(TemplateNames[0]);
	
	LevelDungeon->RegenerateTiles();
}

void FDungeoneerEdModeToolkit::OnSelectTemplate(TSharedPtr<FString> Template, ESelectInfo::Type)
{
	LevelDungeon->SelectedTemplate = FName(*Template);
}

class FEdMode* FDungeoneerEdModeToolkit::GetEditorMode() const
{
	return GLevelEditorModeTools().GetActiveMode(FDungeoneerEdMode::EM_DungeoneerEdModeId);
}

#undef LOCTEXT_NAMESPACE
