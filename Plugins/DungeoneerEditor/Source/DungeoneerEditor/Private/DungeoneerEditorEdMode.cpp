// Copyright Epic Games, Inc. All Rights Reserved.

#include "DungeoneerEditorEdMode.h"
#include "DungeoneerEditorEdModeToolkit.h"
#include "Toolkits/ToolkitManager.h"
#include "EditorModeManager.h"
#include "EngineUtils.h"

const FEditorModeID FDungeoneerEditorEdMode::EM_DungeoneerEditorEdModeId = TEXT("EM_DungeoneerEditorEdMode");

FDungeoneerEditorEdMode::FDungeoneerEditorEdMode()
{

}

FDungeoneerEditorEdMode::~FDungeoneerEditorEdMode()
{
	LevelDungeon = NULL;
}

void FDungeoneerEditorEdMode::Enter()
{
	FEdMode::Enter();

	LevelDungeon = NULL;
	UWorld* World = GEditor->GetEditorWorldContext().World();
	for (TActorIterator<ADungeon> It(World); It; ++It)
	{
		ADungeon* dungeon = (*It);
		if (!LevelDungeon)
		{
			LevelDungeon = dungeon;
			break;
		}
	}
	if (!LevelDungeon)
		LevelDungeon = World->SpawnActor<ADungeon>(ADungeon::StaticClass());
	
		
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




