#pragma once
#include "DungeoneerEditorEdMode.h"
#include "Widgets/SCompoundWidget.h"
#include "DungeoneerTemplateWidget.h"

class SDungeoneerEditorWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDungeoneerEditorWidget) {}
	SLATE_END_ARGS();

	void Construct(const FArguments& InArgs);

	// Util Functions
	class FDungeoneerEditorEdMode* GetEdMode() const;

private:
	TSharedPtr<SDungeoneerTemplateWidget> TemplateWidget;
};