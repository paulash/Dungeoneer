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
	auto NameToCommandMap = Commands.NameToCommandMap;
	
	for (const TPair<FName, TSharedPtr<FUICommandInfo>>& pair : NameToCommandMap)
	{
		CommandList->MapAction(pair.Value,
		FExecuteAction::CreateSP(this, &FDungeoneerEditorEdModeToolkit::OnChangeTool, pair.Key),
			FCanExecuteAction::CreateSP(this, &FDungeoneerEditorEdModeToolkit::IsToolEnabled, pair.Key),
			FIsActionChecked::CreateSP(this, &FDungeoneerEditorEdModeToolkit::IsToolActive, pair.Key)
		);
	}
	
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

void FDungeoneerEditorEdModeToolkit::OnChangeTool(FName _ToolName)
{
	GetEditorMode()->SetCurrentTool(_ToolName);
}

bool FDungeoneerEditorEdModeToolkit::IsToolEnabled(FName _ToolName) const
{
	return true;
}

bool FDungeoneerEditorEdModeToolkit::IsToolActive(FName _ToolName) const
{
	return GetEditorMode()->CurrentTool->GetToolName() == _ToolName;
}

FDungeoneerEditorEdMode* FDungeoneerEditorEdModeToolkit::GetEditorMode() const
{
	return (FDungeoneerEditorEdMode*)GLevelEditorModeTools().GetActiveMode(FDungeoneerEditorEdMode::EM_DungeoneerEditorEdModeId);
}

#undef LOCTEXT_NAMESPACE
