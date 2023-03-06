#pragma once
#include "DungeoneerEditorEdMode.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Views/SListView.h"
#include "IStructureDetailsView.h"

class SDungeoneerTemplateWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDungeoneerTemplateWidget) {}
	SLATE_END_ARGS();

	void Construct(const FArguments& InArgs);

private:

	TSharedRef<ITableRow> OnGenerateRowForTemplateList(TSharedPtr<FString> Item, const TSharedRef<STableViewBase>& OwnerTable)
	{
		return
			SNew(STableRow<TSharedPtr<FString>>, OwnerTable)
			[
				SNew(STextBlock).Text(FText::FromString(*Item))
			];
	};

	void OnSelectTemplateList(TSharedPtr<FString> Item, ESelectInfo::Type);
	
	TSharedPtr<SListView<TSharedPtr<FString>>> TemplateList;
	TArray<TSharedPtr<FString>> TemplateNames;
	FName SelectedTemplate = NAME_None;

	void OnTemplateAdded(FName NewTemplate)
	{
		SelectedTemplate = NewTemplate;
		RefreshTemplateList();	
	};
	void OnTemplateRemoved(FName RemovedTemplate)
	{
		if (SelectedTemplate == RemovedTemplate)
			SelectedTemplate = NAME_None;
		RefreshTemplateList();	
	};
	void OnTemplateRenamed(FName OldTemplateName, FName NewTemplateName)
	{
		if (SelectedTemplate == OldTemplateName)
			SelectedTemplate = NewTemplateName;
		RefreshTemplateList();
	}
	
	void RefreshTemplateList();
	void OnFinishDetails(const FPropertyChangedEvent& evt);

	TSharedPtr<IStructureDetailsView> ModelDetails;

	TSharedPtr<SEditableText> TemplateNameField;

	FReply AddTemplate();
	FReply RemoveTemplate();

	void OnTemplateNameCommit(const FText& text, ETextCommit::Type type);

	EVisibility IsTemplateValid() const
	{
		return (SelectedTemplate != NAME_None) ? EVisibility::Visible : EVisibility::Collapsed;
	}
};