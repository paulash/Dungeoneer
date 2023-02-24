// Copyright Epic Games, Inc. All Rights Reserved.

#include "DungeoneerEdModeToolkit.h"
#include "DungeoneerEdMode.h"
#include "EditorModeManager.h"
#include "Widgets/Input/STextComboBox.h"
#include "IStructureDetailsView.h"

#define LOCTEXT_NAMESPACE "FDungeoneerEdModeToolkit"

FDungeoneerEdModeToolkit::FDungeoneerEdModeToolkit()
{
}

void FDungeoneerEdModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost)
{
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	
	FStructOnScope* ScopeDungeonPalette = new FStructOnScope(FDungeonModel::StaticStruct(), NULL);//(uint8*)&LevelDungeon->DungeonPalette);
	ModelDetails = PropertyEditorModule.CreateStructureDetailView(
		FDetailsViewArgs(),
		FStructureDetailsViewArgs(),
		MakeShareable(ScopeDungeonPalette),
		FText::FromString("Dungeon Model")
	);
	ModelDetails->GetOnFinishedChangingPropertiesDelegate().AddRaw(this, &FDungeoneerEdModeToolkit::OnFinishDetails);

	TemplateList = SNew(SListView<TSharedPtr<FString>>).ListItemsSource(&TemplateNames)
					.OnGenerateRow(this, &FDungeoneerEdModeToolkit::OnGenerateRowForList)
					.OnSelectionChanged(this, &FDungeoneerEdModeToolkit::OnSelectModel);
	RegenerateTemplates();

	TSharedPtr<SButton> AddModel = SNew(SButton)
									.Text(FText::FromString("Add Model"))
									.OnClicked(this, &FDungeoneerEdModeToolkit::AddModel);
	TSharedPtr<SButton> DeleteModel = SNew(SButton)
									.Text(FText::FromString("Delete Model"))
									.OnClicked(this, &FDungeoneerEdModeToolkit::RemoveModel);
	
	SAssignNew(ToolkitWidget, SBorder)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.VAlign(VAlign_Fill)
			[
				TemplateList.ToSharedRef()
			]
			+ SVerticalBox::Slot()
			.VAlign(VAlign_Bottom)
			[
				AddModel.ToSharedRef()
			]
			+ SVerticalBox::Slot()
			.VAlign(VAlign_Fill)
			[
				ModelDetails->GetWidget().ToSharedRef()
			]
			+ SVerticalBox::Slot()
			.VAlign(VAlign_Bottom)
			[
				DeleteModel.ToSharedRef()
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

void FDungeoneerEdModeToolkit::OnSelectModel(TSharedPtr<FString> Item, ESelectInfo::Type)
{
	if (!Item)
	{
		TemplateList->SetSelection(TemplateNames[0]);
		return;
	}
	LevelDungeon->SelectedTemplate = FName(*Item);

	FStructOnScope* ScopeDungeonPalette = new FStructOnScope(FDungeonModel::StaticStruct(),
		(uint8*)&LevelDungeon->DungeonPalette.Models[LevelDungeon->SelectedTemplate]);
	ModelDetails->SetStructureData(MakeShareable(ScopeDungeonPalette));
}

void FDungeoneerEdModeToolkit::OnFinishDetails(const FPropertyChangedEvent& evt)
{
	RegenerateTemplates();
	LevelDungeon->RegenerateTiles();
}

void FDungeoneerEdModeToolkit::RegenerateTemplates()
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
	
	TemplateList->SetSelection(TemplateNames[0]);
}

class FEdMode* FDungeoneerEdModeToolkit::GetEditorMode() const
{
	return GLevelEditorModeTools().GetActiveMode(FDungeoneerEdMode::EM_DungeoneerEdModeId);
}

#undef LOCTEXT_NAMESPACE
