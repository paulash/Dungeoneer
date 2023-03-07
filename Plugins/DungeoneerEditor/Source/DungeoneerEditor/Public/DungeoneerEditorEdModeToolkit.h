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
	virtual FDungeoneerEditorEdMode* GetEditorMode() const override;
	virtual TSharedPtr<class SWidget> GetInlineContent() const override { return ToolkitWidget; }

	virtual void BuildToolPalette(FName Palette, FToolBarBuilder& ToolbarBuilder) override;
	virtual void GetToolPaletteNames(TArray<FName>& PaletteNames) const override;
	virtual void OnToolPaletteChanged(FName PaletteName) override;

	TSharedPtr<SDungeoneerEditorWidget> ToolkitWidget;
	
private:

	void OnChangeTool(FName _ToolName);
	bool IsToolEnabled(FName _ToolName) const;
	bool IsToolActive(FName _ToolName) const;
};
