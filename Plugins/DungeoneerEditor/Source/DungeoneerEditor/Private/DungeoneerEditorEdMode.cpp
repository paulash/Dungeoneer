// Copyright Epic Games, Inc. All Rights Reserved.

#include "DungeoneerEditorEdMode.h"
#include "DungeoneerEditorEdModeToolkit.h"
#include "Toolkits/ToolkitManager.h"
#include "EditorModeManager.h"

const FEditorModeID FDungeoneerEditorEdMode::EM_DungeoneerEditorEdModeId = TEXT("EM_DungeoneerEditorEdMode");

FDungeoneerEditorEdMode::FDungeoneerEditorEdMode()
{

}

FDungeoneerEditorEdMode::~FDungeoneerEditorEdMode()
{

}

void FDungeoneerEditorEdMode::Enter()
{
	FEdMode::Enter();

	if (!Toolkit.IsValid() && UsesToolkits())
	{
		Toolkit = MakeShareable(new FDungeoneerEditorEdModeToolkit);
		Toolkit->Init(Owner->GetToolkitHost());
	}
}

void FDungeoneerEditorEdMode::Exit()
{
	if (Toolkit.IsValid())
	{
		FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
		Toolkit.Reset();
	}

	// Call base Exit method to ensure proper cleanup
	FEdMode::Exit();
}

bool FDungeoneerEditorEdMode::UsesToolkits() const
{
	return true;
}




