// Copyright Epic Games, Inc. All Rights Reserved.

#include "Dungeoneer.h"
#include "DungeoneerEdMode.h"

#define LOCTEXT_NAMESPACE "FDungeoneerModule"

void FDungeoneerModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FEditorModeRegistry::Get().RegisterMode<FDungeoneerEdMode>(FDungeoneerEdMode::EM_DungeoneerEdModeId, LOCTEXT("DungeoneerEdModeName", "DungeoneerEdMode"), FSlateIcon(), true);
}

void FDungeoneerModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FEditorModeRegistry::Get().UnregisterMode(FDungeoneerEdMode::EM_DungeoneerEdModeId);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDungeoneerModule, Dungeoneer)