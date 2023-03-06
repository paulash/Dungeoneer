#include "DungeoneerTemplateCombo.h"
#include "DungeoneerEditorEdMode.h"

void SDungeoneerTemplateCombo::Construct(const FArguments& InArgs)
{
	FDungeoneerEditorEdMode::GetEdMode()->OnTemplateAdded.AddRaw(this, &SDungeoneerTemplateCombo::OnTemplateAdded);
	FDungeoneerEditorEdMode::GetEdMode()->OnTemplateRemoved.AddRaw(this, &SDungeoneerTemplateCombo::OnTemplateRemoved);
	FDungeoneerEditorEdMode::GetEdMode()->OnTemplateRenamed.AddRaw(this, &SDungeoneerTemplateCombo::OnTemplateRenamed);

	ComboBox = SNew(SComboBox<FComboItemType>)
		.OptionsSource(&Options)
		.OnSelectionChanged(this, &SDungeoneerTemplateCombo::OnSelectionChanged)
		.OnGenerateWidget(this, &SDungeoneerTemplateCombo::MakeWidgetForOption)
		.InitiallySelectedItem(CurrentItem)
		[
			SNew(STextBlock)
			.Text(this, &SDungeoneerTemplateCombo::GetCurrentItemLabel)
		];
	RefreshTemplates();
	CurrentItem = Options[0];
	
	ChildSlot
	[
		ComboBox.ToSharedRef()
	];
}

void SDungeoneerTemplateCombo::OnSelectionChanged(FComboItemType NewValue, ESelectInfo::Type)
{
	CurrentItem = NewValue;
}

void SDungeoneerTemplateCombo::RefreshTemplates()
{
	Options.Empty();
	if (FDungeoneerEditorEdMode::GetEdMode()->LevelDungeon->DungeonPalette.Models.Num() == 0)
	{
		Options.Add(MakeShareable(new FString("NO TEMPLATES!")));	
	}
	else
	{
		TArray<FName> TemplateNames;
		FDungeoneerEditorEdMode::GetEdMode()->LevelDungeon->DungeonPalette.Models.GenerateKeyArray(TemplateNames);

		for (int i=0; i < TemplateNames.Num(); i++)
			Options.Add(MakeShareable(new FString(TemplateNames[i].ToString())));
	}
	ComboBox->RefreshOptions();
}
