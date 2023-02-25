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

	TSharedPtr<SButton> AddModel = SNew(SButton)
									.Text(FText::FromString("Add Template"))
									.OnClicked(this, &SDungeoneerEditorWidget::AddModel);
	TSharedPtr<SButton> RemoveModel = SNew(SButton)
									.Text(FText::FromString("Remove Template"))
									.OnClicked(this, &SDungeoneerEditorWidget::RemoveModel);
	
	TemplateNameField = SNew(SEditableText)
							.OnTextCommitted(this, &SDungeoneerEditorWidget::OnTemplateNameCommit);

	//TemplateList.Visibility(this, &SDungeoneerEditorWidget::IsTemplateValid);
	
	ChildSlot
	[
		SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.VAlign(VAlign_Top)
			.AutoHeight()
			[
				SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.HAlign(HAlign_Fill)
					[
						AddModel.ToSharedRef()
					]
					+ SHorizontalBox::Slot()
					.HAlign(HAlign_Fill)
					[
						RemoveModel.ToSharedRef()
					]
			]
			+ SVerticalBox::Slot()
			.VAlign(VAlign_Fill)
			[
				TemplateList.ToSharedRef()
			]
			+ SVerticalBox::Slot()
			.VAlign(VAlign_Bottom)
			[
				SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SHorizontalBox)
							.Visibility(this, &SDungeoneerEditorWidget::IsTemplateValid)
							+ SHorizontalBox::Slot()
							.HAlign(HAlign_Left)
							.AutoWidth()
							[
								SNew(STextBlock).Text(FText::FromString("TEMPLATE NAME:"))
							]
							+ SHorizontalBox::Slot()
							.HAlign(HAlign_Fill)
							[
								TemplateNameField.ToSharedRef()
							]
					]
					+ SVerticalBox::Slot()
					[
						ModelDetails->GetWidget().ToSharedRef()
					]
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
		TemplateNameField->SetText(FText::FromString(""));
		GetEdMode()->SelectedTemplate = NAME_None;
		return;
	}
	GetEdMode()->SelectedTemplate = FName(*Item);
	SelectedTemplateName = GetEdMode()->SelectedTemplate;

	TemplateNameField->SetText(FText::FromString(SelectedTemplateName.ToString()));

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

	TemplateList->ReGenerateItems(TemplateList->GetCachedGeometry());
}

void SDungeoneerEditorWidget::OnFinishDetails(const FPropertyChangedEvent& evt)
{
	RefreshTemplateList();
	GetEdMode()->LevelDungeon->RegenerateTiles();
}

FReply SDungeoneerEditorWidget::AddModel()
{
	GetEdMode()->LevelDungeon->DungeonPalette.Models.Emplace("NEW", FDungeonModel());
	RefreshTemplateList();
	return FReply::Handled();
}

FReply SDungeoneerEditorWidget::RemoveModel()
{
	GetEdMode()->LevelDungeon->DungeonPalette.Models.Remove(GetEdMode()->SelectedTemplate);
	RefreshTemplateList();
	return FReply::Handled();
}

void SDungeoneerEditorWidget::OnTemplateNameCommit(const FText& text, ETextCommit::Type type)
{
	if (GetEdMode()->SelectedTemplate != NAME_None)
	{
		FDungeonModel model = GetEdMode()->LevelDungeon->DungeonPalette.Models[GetEdMode()->SelectedTemplate];
		GetEdMode()->LevelDungeon->DungeonPalette.Models.Remove(GetEdMode()->SelectedTemplate);
		GetEdMode()->LevelDungeon->DungeonPalette.Models.Emplace(FName(text.ToString()), model);
		GetEdMode()->SelectedTemplate = FName(text.ToString());

		RefreshTemplateList();
		for (int i=0; i < TemplateNames.Num(); i++)
		{
			FName checkName = FName(*TemplateNames[i]);
			if (checkName == GetEdMode()->SelectedTemplate)
			{
				TemplateList->SetSelection(TemplateNames[i]);
				break;
			}
		}
	}
}
