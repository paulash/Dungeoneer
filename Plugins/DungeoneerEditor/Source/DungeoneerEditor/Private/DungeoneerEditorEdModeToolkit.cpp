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
	const auto& Commands = FDungeoneerEditorCommands::Get();
	TSharedRef<FUICommandList> CommandList = GetToolkitCommands();

	//#define MAP_TOOL(ToolName) CommandList->MapAction(NameToCommandMap.FindChecked("Tool_" ToolName), FUIAction(FExecuteAction::CreateSP(this, &FLandscapeToolKit::OnChangeTool, FName(ToolName)), FCanExecuteAction::CreateSP(this, &FLandscapeToolKit::IsToolEnabled, FName(ToolName)), FIsActionChecked::CreateSP(this, &FLandscapeToolKit::IsToolActive, FName(ToolName)), FIsActionButtonVisible::CreateSP(this, &FLandscapeToolKit::IsToolAvailable, FName(ToolName))));
	
	CommandList->MapAction(
		Commands.SelectTool,
		FUIAction(FExecuteAction::CreateSP(this, &FDungeoneerEditorEdModeToolkit::OnChangeTool, FName("TOOL_PAINT"))));
	
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
	ToolbarBuilder.AddToolBarButton(Commands.PaintTool);
	ToolbarBuilder.EndSection();
	
	FModeToolkit::BuildToolPalette(Palette, ToolbarBuilder);
}

void FDungeoneerEditorEdModeToolkit::GetToolPaletteNames(TArray<FName>& PaletteNames) const
{
	if (!GetDefault<UEditorStyleSettings>()->bEnableLegacyEditorModeUI)
	{
		PaletteNames = { FName("Managed") };
	}
}

void FDungeoneerEditorEdModeToolkit::OnToolPaletteChanged(FName PaletteName)
{
	FModeToolkit::OnToolPaletteChanged(PaletteName);
}

void FDungeoneerEditorEdModeToolkit::OnChangeTool(FName _NewToolName)
{
}

class FEdMode* FDungeoneerEditorEdModeToolkit::GetEditorMode() const
{
	return GLevelEditorModeTools().GetActiveMode(FDungeoneerEditorEdMode::EM_DungeoneerEditorEdModeId);
}

#undef LOCTEXT_NAMESPACE
