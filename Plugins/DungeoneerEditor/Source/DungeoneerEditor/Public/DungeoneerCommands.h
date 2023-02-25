#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"

class FDungeoneerEditorCommands : public TCommands<FDungeoneerEditorCommands>
{
public:
	FDungeoneerEditorCommands();
	

	/**
	 * Initialize commands
	 */
	virtual void RegisterCommands() override;

	static FName DungeoneerContext;

	TSharedPtr<FUICommandInfo> SelectTool;
	
	TMap<FName, TSharedPtr<FUICommandInfo>> NameToCommandMap;
};