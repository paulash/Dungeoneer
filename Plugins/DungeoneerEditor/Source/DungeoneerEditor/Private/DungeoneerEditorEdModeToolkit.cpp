#include "DungeoneerEditorEdModeToolkit.h"

#include "DungeoneerCommands.h"
#include "DungeoneerEditorEdMode.h"
#include "Classes/EditorStyleSettings.h"
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

void FDungeoneerEditorEdModeToolkit::BuildToolPalette(FName Palette, FToolBarBuilder& ToolbarBuilder)
{
	auto Commands = FDungeoneerEditorCommands::Get();
	
	ToolbarBuilder.BeginSection("Manage");
	ToolbarBuilder.AddToolBarButton(Commands.SelectTool);
	ToolbarBuilder.AddSeparator();
	
	FModeToolkit::BuildToolPalette(Palette, ToolbarBuilder);
}

void FDungeoneerEditorEdModeToolkit::GetToolPaletteNames(TArray<FName>& PaletteNames) const
{
	if (!GetDefault<UEditorStyleSettings>()->bEnableLegacyEditorModeUI)
	{
		PaletteNames = { FName("Managed") };
	}
}

class FEdMode* FDungeoneerEditorEdModeToolkit::GetEditorMode() const
{
	return GLevelEditorModeTools().GetActiveMode(FDungeoneerEditorEdMode::EM_DungeoneerEditorEdModeId);
}

#undef LOCTEXT_NAMESPACE
