// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Toolkits/BaseToolkit.h"
#include "Widgets/Views/SListView.h"
#include "Dungeon.h"


class FDungeoneerEdModeToolkit : public FModeToolkit
{
public:

	FDungeoneerEdModeToolkit();
	
	/** FModeToolkit interface */
	virtual void Init(const TSharedPtr<IToolkitHost>& InitToolkitHost) override;

	/** IToolkit interface */
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual class FEdMode* GetEditorMode() const override;
	virtual TSharedPtr<class SWidget> GetInlineContent() const override
	{
		return ToolkitWidget;
	};

	ADungeon* LevelDungeon = NULL;
	
private:

	TSharedRef<ITableRow> OnGenerateRowForList(TSharedPtr<FString> Item, const TSharedRef<STableViewBase>& OwnerTable)
	{
		return
			SNew(STableRow<TSharedPtr<FString>>, OwnerTable)
			[
				SNew(STextBlock).Text(FText::FromString(*Item))
			];
	}

	void OnSelectModel(TSharedPtr<FString> Item, ESelectInfo::Type);
	void OnFinishDetails(const FPropertyChangedEvent& evt);
	
	FReply AddModel()
	{
		LevelDungeon->DungeonPalette.Models.Emplace("NEW", FDungeonModel());
		RegenerateTemplates();
		return FReply::Handled();
	};

	FReply RemoveModel()
	{
		LevelDungeon->DungeonPalette.Models.Remove(LevelDungeon->SelectedTemplate);
		RegenerateTemplates();
		return FReply::Handled();	
	};

	void RegenerateTemplates();

	TSharedPtr<IStructureDetailsView> ModelDetails;

	TSharedPtr<SListView<TSharedPtr<FString>>> TemplateList;
	
	TSharedPtr<SWidget> ToolkitWidget;
	TArray<TSharedPtr<FString>> TemplateNames;
};
