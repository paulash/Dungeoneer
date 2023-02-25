#pragma once
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Views/SListView.h"
#include "IStructureDetailsView.h"

class SDungeoneerEditorWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDungeoneerEditorWidget) {}
	SLATE_END_ARGS();

	void Construct(const FArguments& InArgs);

	// Util Functions
	class FDungeoneerEditorEdMode* GetEdMode() const;

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
	FName SelectedTemplateName = NAME_None;
	
	void RefreshTemplateList();
	void OnFinishDetails(const FPropertyChangedEvent& evt);

	TSharedPtr<IStructureDetailsView> ModelDetails;

	TSharedPtr<SEditableText> TemplateNameField;

	FReply AddModel();
	FReply RemoveModel();
};