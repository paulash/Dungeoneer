#pragma once
#include "DungeoneerEditorEdMode.h"
#include "Widgets/SCompoundWidget.h"
#include "IStructureDetailsView.h"

#define LOCTEXT_NAMESPACE "DungeoneerTemplateCombo"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSelectTemplate, FName /*, TemplateName*/);

class SDungeoneerTemplateCombo : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDungeoneerTemplateCombo)
	{}
	SLATE_END_ARGS()

	FOnSelectTemplate OnSelectTemplate;

	void Construct(const FArguments& InArgs);

	TSharedRef<SWidget> MakeWidgetForOption(TSharedPtr<FString>  InOption)
	{
		return SNew(STextBlock).Text(FText::FromString(*InOption));
	}

	void OnSelectionChanged(TSharedPtr<FString> NewValue, ESelectInfo::Type);

	FText GetCurrentItemLabel() const
	{
		return FText::FromString(SelectedTemplate.ToString());
	}

	void SetSelection(FName TemplateName);

protected:

	void OnTemplateAdded(FName NewTemplate)
	{
		RefreshTemplates();
	}
	void OnTemplateRemoved(FName RemovedTemplate)
	{
		RefreshTemplates();
	}
	void OnTemplateRenamed(FName OldTemplate, FName NewTemplate)
	{
		RefreshTemplates();
	}
	
	void RefreshTemplates();

	FName SelectedTemplate = "NONE";
	TArray<TSharedPtr<FString>> Templates;
	TSharedPtr<SComboBox<TSharedPtr<FString>>> ComboBox;
	
};

#undef LOCTEXT_NAMESPACE
