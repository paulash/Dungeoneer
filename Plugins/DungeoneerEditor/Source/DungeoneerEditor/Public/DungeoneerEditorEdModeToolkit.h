#pragma once

#include "CoreMinimal.h"
#include "Toolkits/BaseToolkit.h"
#include "DungeoneerEditorWidget.h"

class FDungeoneerEditorEdModeToolkit : public FModeToolkit
{
public:

	FDungeoneerEditorEdModeToolkit();
	
	/** FModeToolkit interface */
	virtual void Init(const TSharedPtr<IToolkitHost>& InitToolkitHost) override;

	/** IToolkit interface */
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual class FEdMode* GetEditorMode() const override;
	virtual TSharedPtr<class SWidget> GetInlineContent() const override { return ToolkitWidget; }

private:

	TSharedPtr<SDungeoneerEditorWidget> ToolkitWidget;
};