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

	CommandList->MapAction(
		Commands.SelectTool,
		FExecuteAction::CreateSP(this, &FDungeoneerEditorEdModeToolkit::SetSelectTool));
	
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

void FDungeoneerEditorEdModeToolkit::SetSelectTool()
{
	
}

class FEdMode* FDungeoneerEditorEdModeToolkit::GetEditorMode() const
{
	return GLevelEditorModeTools().GetActiveMode(FDungeoneerEditorEdMode::EM_DungeoneerEditorEdModeId);
}

#undef LOCTEXT_NAMESPACE
