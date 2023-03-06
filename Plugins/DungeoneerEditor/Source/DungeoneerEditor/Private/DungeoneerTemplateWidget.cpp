#include "DungeoneerTemplateWidget.h"
#include "DungeoneerEditor.h"
#include "DungeoneerEditorEdMode.h"
#include "Widgets/Layout/SScrollBox.h"
#include "EditorModeManager.h"

#define LOCTEXT_NAMESPACE "DungeoneerTemplateWidget"

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

	// hook the template events.
	FDungeoneerEditorEdMode::GetEdMode()->OnTemplateAdded.AddRaw(this, &SDungeoneerTemplateWidget::OnTemplateAdded);
	FDungeoneerEditorEdMode::GetEdMode()->OnTemplateRemoved.AddRaw(this, &SDungeoneerTemplateWidget::OnTemplateRemoved);
	FDungeoneerEditorEdMode::GetEdMode()->OnTemplateRenamed.AddRaw(this, &SDungeoneerTemplateWidget::OnTemplateRenamed);

	TSharedPtr<SButton> AddModel = SNew(SButton)
									.Text(FText::FromString("Add Template"))
									.OnClicked(this, &SDungeoneerTemplateWidget::AddTemplate);
	TSharedPtr<SButton> RemoveModel = SNew(SButton)
									.Text(FText::FromString("Remove Template"))
									.OnClicked(this, &SDungeoneerTemplateWidget::RemoveTemplate);
	
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

void SDungeoneerTemplateWidget::OnSelectTemplateList(TSharedPtr<FString> Item, ESelectInfo::Type)
{
	if (!Item)
	{
		ModelDetails->SetStructureData(NULL);
		TemplateNameField->SetText(FText::FromString(""));
		SelectedTemplate = NAME_None;
		return;
	}
	SelectedTemplate = FName(*Item);
	TemplateNameField->SetText(FText::FromString(SelectedTemplate.ToString()));

	FStructOnScope* ScopeDungeonPalette = new FStructOnScope(FDungeonModel::StaticStruct(),
		(uint8*)&FDungeoneerEditorEdMode::GetEdMode()->LevelDungeon->DungeonPalette.Models[SelectedTemplate]);
	ModelDetails->SetStructureData(MakeShareable(ScopeDungeonPalette));
}

void SDungeoneerTemplateWidget::RefreshTemplateList()
{
	TemplateNames.Empty();
	TArray<FName> _TemplateNames;
	FDungeoneerEditorEdMode::GetEdMode()->LevelDungeon->DungeonPalette.Models.GetKeys(_TemplateNames);
	for (int i=0; i < _TemplateNames.Num(); i++)
	{
		FString name = _TemplateNames[i].ToString();
		FString* sName = new FString(name);
		TemplateNames.Add(MakeShareable(sName));	
	}
	if (TemplateNames.Num() == 0)
		TemplateNames.Add(MakeShareable(new FString("NO TEMPLATES!")));

	TemplateList->ReGenerateItems(TemplateList->GetCachedGeometry());

	// reselect the selected template.
	if (SelectedTemplate != NAME_None)
	{
		for (int i=0; i < TemplateNames.Num(); i++)
		{
			FName checkName = FName(*TemplateNames[i]);
			if (checkName == SelectedTemplate)
			{
				TemplateList->SetSelection(TemplateNames[i]);
				break;
			}
		}
	}
}

void SDungeoneerTemplateWidget::OnFinishDetails(const FPropertyChangedEvent& evt)
{
	RefreshTemplateList();
	FDungeoneerEditorEdMode::GetEdMode()->LevelDungeon->RegenerateTiles();
}

FReply SDungeoneerTemplateWidget::AddTemplate()
{
	FString guid = FGuid::NewGuid().ToString(EGuidFormats::Short);
	FDungeoneerEditorEdMode::GetEdMode()->AddTemplate(FName("NEW_" + guid), FDungeonModel());
	return FReply::Handled();
}

FReply SDungeoneerTemplateWidget::RemoveTemplate()
{
	if (SelectedTemplate != NAME_None)
	{
		FText msg = LOCTEXT("WARN_REMOVE_TEMPLATE", "Are you sure you wish to remove this template? Any references in tiles will lose there connection.");
		if (FMessageDialog::Open(EAppMsgType::OkCancel, msg) == EAppReturnType::Ok)
		{
			FDungeoneerEditorEdMode::GetEdMode()->RemoveTemplate(SelectedTemplate);
			TemplateList->SetSelection(NULL);	
		}
	}
	return FReply::Handled();
}

void SDungeoneerTemplateWidget::OnTemplateNameCommit(const FText& text, ETextCommit::Type type)
{
	if (SelectedTemplate != NAME_None)
	{
		FDungeoneerEditorEdMode::GetEdMode()->RenameTemplate(SelectedTemplate,FName(text.ToString()));
	}
}
