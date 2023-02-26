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
	UI_COMMAND(SelectTool, "Select", "Select tiles, shift click to multi-select", EUserInterfaceActionType::RadioButton, FInputChord());
	NameToCommandMap.Add("Tool_Select", SelectTool);

	UI_COMMAND(PaintTool, "Paint", "Select tiles, shift click to multi-select", EUserInterfaceActionType::RadioButton, FInputChord());
	NameToCommandMap.Add("Tool_Paint", PaintTool);
}
