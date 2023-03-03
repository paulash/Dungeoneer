#define LOCTEXT_NAMESPACE "DungeoneerTemplateCombo"

class SDungeoneerTemplateCombo : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDungeoneerTemplateCombo)
	{}
	SLATE_END_ARGS()

	typedef TSharedPtr<FString> FComboItemType;
	
	void Construct(const FArguments& InArgs)
	{
		/*
		Options.Add(MakeShareable(new FString("Option1")));
		Options.Add(MakeShareable(new FString("Option2")));
		Options.Add(MakeShareable(new FString("LastOption")));
		*/

		Options.Add(MakeShareable(new FString("")));
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

	TSharedRef<SWidget> MakeWidgetForOption(FComboItemType InOption)
	{
		return SNew(STextBlock).Text(FText::FromString(*InOption));
	}

	void OnSelectionChanged(FComboItemType NewValue, ESelectInfo::Type)
	{
		CurrentItem = NewValue;
	}

	FText GetCurrentItemLabel() const
	{
		if (CurrentItem.IsValid())
		{
			return FText::FromString(*CurrentItem);
		}

		return LOCTEXT("InvalidComboEntryText", "<<Invalid option>>");
	}

	FComboItemType CurrentItem;
	TArray<FComboItemType> Options;
};

#undef LOCTEXT_NAMESPACE