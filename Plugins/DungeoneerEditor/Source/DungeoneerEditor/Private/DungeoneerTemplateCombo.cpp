#include "DungeoneerTemplateCombo.h"
#include "DungeoneerEditorEdMode.h"

void SDungeoneerTemplateCombo::Construct(const FArguments& InArgs)
{
	FDungeoneerEditorEdMode::GetEdMode()->OnTemplateAdded.AddRaw(this, &SDungeoneerTemplateCombo::OnTemplateAdded);
	FDungeoneerEditorEdMode::GetEdMode()->OnTemplateRemoved.AddRaw(this, &SDungeoneerTemplateCombo::OnTemplateRemoved);
	FDungeoneerEditorEdMode::GetEdMode()->OnTemplateRenamed.AddRaw(this, &SDungeoneerTemplateCombo::OnTemplateRenamed);

	RefreshTemplates();
	ComboBox = SNew(SComboBox<TSharedPtr<FString>>)
		.OptionsSource(&Templates)
		.OnSelectionChanged(this, &SDungeoneerTemplateCombo::OnSelectionChanged)
		.OnGenerateWidget(this, &SDungeoneerTemplateCombo::MakeWidgetForOption)
		.InitiallySelectedItem(Templates[0])
		[
			SNew(STextBlock)
			.Text(this, &SDungeoneerTemplateCombo::GetCurrentItemLabel)
		];
	SelectedTemplate = FName(*Templates[0]);
	
	ChildSlot
	[
		ComboBox.ToSharedRef()
	];
}

void SDungeoneerTemplateCombo::OnSelectionChanged(TSharedPtr<FString> NewValue, ESelectInfo::Type)
{
	if (!NewValue.IsValid())
	{
		SelectedTemplate = "NONE";
		return;
	}
	SelectedTemplate = FName(*NewValue);
	OnSelectTemplate.Broadcast(SelectedTemplate);
}

void SDungeoneerTemplateCombo::SetSelection(FName TemplateName)
{
	SelectedTemplate = TemplateName;
}

void SDungeoneerTemplateCombo::RefreshTemplates()
{
	Templates.Empty();
	if (FDungeoneerEditorEdMode::GetEdMode()->LevelDungeon->DungeonPalette.Models.Num() == 0)
	{
		Templates.Add(MakeShareable(new FString("NO TEMPLATES!")));	
	}
	else
	{
		TArray<FName> TemplateNames;
		FDungeoneerEditorEdMode::GetEdMode()->LevelDungeon->DungeonPalette.Models.GenerateKeyArray(TemplateNames);

		Templates.Add(MakeShareable(new FString("NONE")));
		for (int i=0; i < TemplateNames.Num(); i++)
			Templates.Add(MakeShareable(new FString(TemplateNames[i].ToString())));
	}
	if (ComboBox)
		ComboBox->RefreshOptions();
}
