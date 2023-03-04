#include "DungeoneerTemplateCombo.h"
#include "DungeoneerEditorEdMode.h"

void SDungeoneerTemplateCombo::Construct(const FArguments& InArgs)
{
	FDungeoneerEditorEdMode::GetEdMode()->OnTemplateUpdated.AddRaw(this, &SDungeoneerTemplateCombo::RefreshTemplates);
	
	RefreshTemplates();
	CurrentItem = Options[0];

	

	ChildSlot
		[
			SNew(SComboBox<FComboItemType>)
			.OptionsSource(&Options)
			.OnSelectionChanged(this, &SDungeoneerTemplateCombo::OnSelectionChanged)
			.OnGenerateWidget(this, &SDungeoneerTemplateCombo::MakeWidgetForOption)
			.InitiallySelectedItem(CurrentItem)
			[
				SNew(STextBlock)
				.Text(this, &SDungeoneerTemplateCombo::GetCurrentItemLabel)
			]
		];
}

void SDungeoneerTemplateCombo::RefreshTemplates()
{
	if (FDungeoneerEditorEdMode::GetEdMode()->LevelDungeon->DungeonPalette.Models.Num() == 0)
	{
		Options.Add(MakeShareable(new FString("NO TEMPLATES!")));	
	}
	else
	{
		
	}
}
