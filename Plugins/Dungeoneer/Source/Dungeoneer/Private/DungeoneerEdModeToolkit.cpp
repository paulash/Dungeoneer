#include "DungeoneerEdModeToolkit.h"
#include "DungeoneerEdMode.h"
#include "Engine/Selection.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "EditorModeManager.h"

#define LOCTEXT_NAMESPACE "FDungeoneerEdModeToolkit"

FDungeoneerEdModeToolkit::FDungeoneerEdModeToolkit()
{
}

void FDungeoneerEdModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost)
{
	SAssignNew(ToolkitWidget, SBorder)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(STextBlock)
				.AutoWrapText(true)
				.Text(LOCTEXT("HelperLabel", "Select some actors and move them around using buttons below"))
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(STextBlock)
				.AutoWrapText(true)
				.Text(LOCTEXT("HelperLabel", "Select some actors and move them around using buttons below"))
			]
		];
	FModeToolkit::Init(InitToolkitHost);
}

FName FDungeoneerEdModeToolkit::GetToolkitFName() const
{
	return FName("DungeoneerEdMode");
}

FText FDungeoneerEdModeToolkit::GetBaseToolkitName() const
{
	return NSLOCTEXT("DungeoneerEdModeToolkit", "DisplayName", "DungeoneerEdMode Tool");
}

class FEdMode* FDungeoneerEdModeToolkit::GetEditorMode() const
{
	return GLevelEditorModeTools().GetActiveMode(FDungeoneerEdMode::EM_DungeoneerEdModeId);
}

#undef LOCTEXT_NAMESPACE
