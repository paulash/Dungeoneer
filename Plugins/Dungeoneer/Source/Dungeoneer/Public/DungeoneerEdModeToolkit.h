// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Toolkits/BaseToolkit.h"

class ADungeon;

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

	TSharedPtr<IStructureDetailsView> PaletteDetails;
	
	TSharedPtr<SWidget> ToolkitWidget;
	TArray<TSharedPtr<FString>> TemplateNames;
};
