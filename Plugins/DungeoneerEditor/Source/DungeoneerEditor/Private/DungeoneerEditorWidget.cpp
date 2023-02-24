#include "DungeoneerEditorWidget.h"
#include "DungeoneerEditor.h"
#include "DungeoneerEditorEdMode.h"
#include "Widgets/Layout/SScrollBox.h"
#include "EditorModeManager.h"

void SDungeoneerEditorWidget::Construct(const FArguments& InArgs)
{

	FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	
	FStructOnScope* ScopeDungeonPalette = new FStructOnScope(FDungeonModel::StaticStruct(), NULL);
	ModelDetails = PropertyEditorModule.CreateStructureDetailView(
		FDetailsViewArgs(),
		FStructureDetailsViewArgs(),
		MakeShareable(ScopeDungeonPalette),
		FText::FromString("Dungeon Model")
	);
	ModelDetails->GetOnFinishedChangingPropertiesDelegate().AddRaw(this, &SDungeoneerEditorWidget::OnFinishDetails);
	
	TemplateList = SNew(SListView<TSharedPtr<FString>>).ListItemsSource(&TemplateNames)
					.OnGenerateRow(this, &SDungeoneerEditorWidget::OnGenerateRowForTemplateList)
					.OnSelectionChanged(this, &SDungeoneerEditorWidget::OnSelectTemplateList)
					.ReturnFocusToSelection(true);
	RefreshTemplateList();
	
	ChildSlot
	[
		SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.VAlign(VAlign_Fill)
			[
				TemplateList.ToSharedRef()
			]
			+ SVerticalBox::Slot()
			.VAlign(VAlign_Fill)
			[
				ModelDetails->GetWidget().ToSharedRef()
			]
	];

}

FDungeoneerEditorEdMode* SDungeoneerEditorWidget::GetEdMode() const
{
	return (FDungeoneerEditorEdMode*)GLevelEditorModeTools().GetActiveMode(FDungeoneerEditorEdMode::EM_DungeoneerEditorEdModeId);
}

void SDungeoneerEditorWidget::OnSelectTemplateList(TSharedPtr<FString> Item, ESelectInfo::Type)
{
	if (!Item)
	{
		ModelDetails->SetStructureData(NULL);
		return;
	}
	GetEdMode()->SelectedTemplate = FName(*Item);
	SelectedTemplateName = GetEdMode()->SelectedTemplate;

	FStructOnScope* ScopeDungeonPalette = new FStructOnScope(FDungeonModel::StaticStruct(),
		(uint8*)&GetEdMode()->LevelDungeon->DungeonPalette.Models[GetEdMode()->SelectedTemplate]);
	ModelDetails->SetStructureData(MakeShareable(ScopeDungeonPalette));
}

void SDungeoneerEditorWidget::RefreshTemplateList()
{
	TemplateNames.Empty();
	TArray<FName> _TemplateNames;
	GetEdMode()->LevelDungeon->DungeonPalette.Models.GetKeys(_TemplateNames);
	for (int i=0; i < _TemplateNames.Num(); i++)
	{
		FString name = _TemplateNames[i].ToString();
		FString* sName = new FString(name);
		TemplateNames.Add(MakeShareable(sName));	
	}
	if (TemplateNames.Num() == 0)
		TemplateNames.Add(MakeShareable(new FString("NO TEMPLATES!")));
}

void SDungeoneerEditorWidget::OnFinishDetails(const FPropertyChangedEvent& evt)
{
	RefreshTemplateList();
	GetEdMode()->LevelDungeon->RegenerateTiles();
}