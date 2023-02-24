#include "DungeoneerEditorEdModeToolkit.h"
#include "DungeoneerEditorEdMode.h"
#include "Engine/Selection.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "EditorModeManager.h"

#define LOCTEXT_NAMESPACE "FDungeoneerEditorEdModeToolkit"

FDungeoneerEditorEdModeToolkit::FDungeoneerEditorEdModeToolkit()
{
}

void FDungeoneerEditorEdModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost)
{
	SAssignNew(ToolkitWidget, SDungeoneerEditorWidget);
	FModeToolkit::Init(InitToolkitHost);
}

FName FDungeoneerEditorEdModeToolkit::GetToolkitFName() const
{
	return FName("DungeoneerEditorEdMode");
}

FText FDungeoneerEditorEdModeToolkit::GetBaseToolkitName() const
{
	return NSLOCTEXT("DungeoneerEditorEdModeToolkit", "DisplayName", "DungeoneerEditorEdMode Tool");
}

class FEdMode* FDungeoneerEditorEdModeToolkit::GetEditorMode() const
{
	return GLevelEditorModeTools().GetActiveMode(FDungeoneerEditorEdMode::EM_DungeoneerEditorEdModeId);
}

#undef LOCTEXT_NAMESPACE
