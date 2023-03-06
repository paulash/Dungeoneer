#define LOCTEXT_NAMESPACE "DungeoneerTemplateCombo"

class SDungeoneerTemplateCombo : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDungeoneerTemplateCombo)
	{}
	SLATE_END_ARGS()

	typedef TSharedPtr<FString> FComboItemType;
	
	void Construct(const FArguments& InArgs);

	TSharedRef<SWidget> MakeWidgetForOption(FComboItemType InOption)
	{
		return SNew(STextBlock).Text(FText::FromString(*InOption));
	}

	void OnSelectionChanged(FComboItemType NewValue, ESelectInfo::Type);

	FText GetCurrentItemLabel() const
	{
		if (CurrentItem.IsValid())
		{
			return FText::FromString(*CurrentItem);
		}

		return LOCTEXT("InvalidComboEntryText", "<<Invalid option>>");
	}

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

	FComboItemType CurrentItem;
	TArray<FComboItemType> Options;

	TSharedPtr<SComboBox<FComboItemType>> ComboBox;
	
};

#undef LOCTEXT_NAMESPACE
