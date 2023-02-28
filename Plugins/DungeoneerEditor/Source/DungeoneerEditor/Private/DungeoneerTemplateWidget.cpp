#include "DungeoneerTemplateWidget.h"
#include "DungeoneerEditor.h"
#include "DungeoneerEditorEdMode.h"
#include "Widgets/Layout/SScrollBox.h"
#include "EditorModeManager.h"

#define LOCTEXT_NAMESPACE "FDungeoneerEditorModule"

void SDungeoneerTemplateWidget::Construct(const FArguments& InArgs)
{
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");

	FDetailsViewArgs args;
	args.bAllowSearch = false;
	args.bShowActorLabel = false;
	args.bAllowMultipleTopLevelObjects = false;
	
	FStructOnScope* ScopeDungeonPalette = new FStructOnScope(FDungeonModel::StaticStruct(), NULL);
	ModelDetails = PropertyEditorModule.CreateStructureDetailView(
		args,
		FStructureDetailsViewArgs(),
		MakeShareable(ScopeDungeonPalette),
		FText::FromString("Dungeon Model")
	);
	ModelDetails->GetOnFinishedChangingPropertiesDelegate().AddRaw(this, &SDungeoneerTemplateWidget::OnFinishDetails);
	
	TemplateList = SNew(SListView<TSharedPtr<FString>>).ListItemsSource(&TemplateNames)
					.OnGenerateRow(this, &SDungeoneerTemplateWidget::OnGenerateRowForTemplateList)
					.OnSelectionChanged(this, &SDungeoneerTemplateWidget::OnSelectTemplateList)
					.ReturnFocusToSelection(true);
	RefreshTemplateList();

	TSharedPtr<SButton> AddModel = SNew(SButton)
									.Text(FText::FromString("Add Template"))
									.OnClicked(this, &SDungeoneerTemplateWidget::AddModel);
	TSharedPtr<SButton> RemoveModel = SNew(SButton)
									.Text(FText::FromString("Remove Template"))
									.OnClicked(this, &SDungeoneerTemplateWidget::RemoveModel);
	
	TemplateNameField = SNew(SEditableText)
							.OnTextCommitted(this, &SDungeoneerTemplateWidget::OnTemplateNameCommit);
	
	
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
				SNew(SBorder)
					.BorderImage(FEditorStyle::GetBrush(TEXT("Menu.Background")))
					.Visibility(this, &SDungeoneerTemplateWidget::IsTemplateValid)
					.Padding(4)
					.VAlign(VAlign_Fill)
					[
						SNew(SVerticalBox)
						+ SVerticalBox::Slot()
							.AutoHeight()
							[
								SNew(STextBlock).Text(LOCTEXT("FDungeoneerEditorModule", "Template Details"))
									.TextStyle(FEditorStyle::Get(), TEXT("Graph.TransitionNode.TooltipName"))
							]
							+ SVerticalBox::Slot()
							.AutoHeight()
							.Padding(FMargin(6,2,16,4))
							[
								SNew(SBorder)
									.BorderImage(FEditorStyle::GetBrush(TEXT("PropertyWindow.WindowBorder")))
									[
										TemplateNameField.ToSharedRef()
									]
							]
							+ SVerticalBox::Slot()
							[
								ModelDetails->GetWidget().ToSharedRef()
							]
					]
			]
	];

}

FDungeoneerEditorEdMode* SDungeoneerTemplateWidget::GetEdMode() const
{
	return (FDungeoneerEditorEdMode*)GLevelEditorModeTools().GetActiveMode(FDungeoneerEditorEdMode::EM_DungeoneerEditorEdModeId);
}

void SDungeoneerTemplateWidget::OnSelectTemplateList(TSharedPtr<FString> Item, ESelectInfo::Type)
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

void SDungeoneerTemplateWidget::RefreshTemplateList()
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

void SDungeoneerTemplateWidget::OnFinishDetails(const FPropertyChangedEvent& evt)
{
	RefreshTemplateList();
	GetEdMode()->LevelDungeon->RegenerateTiles();
}

FReply SDungeoneerTemplateWidget::AddModel()
{
	GetEdMode()->LevelDungeon->DungeonPalette.Models.Emplace("NEW", FDungeonModel());
	RefreshTemplateList();
	return FReply::Handled();
}

FReply SDungeoneerTemplateWidget::RemoveModel()
{
	GetEdMode()->LevelDungeon->DungeonPalette.Models.Remove(GetEdMode()->SelectedTemplate);
	GetEdMode()->SelectedTemplate = NAME_None;
	TemplateList->SetSelection(NULL);
	
	RefreshTemplateList();
	return FReply::Handled();
}

void SDungeoneerTemplateWidget::OnTemplateNameCommit(const FText& text, ETextCommit::Type type)
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
		GetEdMode()->LevelDungeon->RegenerateTiles();
	}
}
