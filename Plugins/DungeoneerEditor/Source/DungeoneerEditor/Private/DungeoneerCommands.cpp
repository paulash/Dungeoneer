#include "DungeoneerCommands.h"

#define LOCTEXT_NAMESPACE "DungeoneerEditorCommands"

FName FDungeoneerEditorCommands::DungeoneerContext = TEXT("DungeoneerEditor");

FDungeoneerEditorCommands::FDungeoneerEditorCommands() : TCommands<FDungeoneerEditorCommands>
	(
		FDungeoneerEditorCommands::DungeoneerContext, // Context name for fast lookup
		NSLOCTEXT("Contexts", "DungeoneerEditor", "Dungeoneer Editor"), // Localized context name for displaying
		NAME_None, //"LevelEditor" // Parent
		FEditorStyle::GetStyleSetName() // Icon Style Set
	)
{
}

void FDungeoneerEditorCommands::RegisterCommands()
{
	UI_COMMAND(SelectTool, "Edit", "Edit tiles, shift click to multi-select", EUserInterfaceActionType::RadioButton, FInputChord());
	NameToCommandMap.Add("TOOL_EDIT", SelectTool);

	UI_COMMAND(PaintTool, "Paint", "Paint tiles, shift click to add tiles, ctrl to delete.", EUserInterfaceActionType::RadioButton, FInputChord());
	NameToCommandMap.Add("TOOL_PAINT", PaintTool);
}

#undef LOCTEXT_NAMESPACE