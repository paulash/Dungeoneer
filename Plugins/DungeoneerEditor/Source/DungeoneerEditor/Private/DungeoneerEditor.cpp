// Copyright Epic Games, Inc. All Rights Reserved.

#include "DungeoneerEditor.h"
#include "DungeoneerEditorEdMode.h"

#define LOCTEXT_NAMESPACE "FDungeoneerEditorModule"

void FDungeoneerEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FEditorModeRegistry::Get().RegisterMode<FDungeoneerEditorEdMode>(FDungeoneerEditorEdMode::EM_DungeoneerEditorEdModeId, LOCTEXT("DungeoneerEditorEdModeName", "DungeoneerEditorEdMode"), FSlateIcon(), true);
}

void FDungeoneerEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FEditorModeRegistry::Get().UnregisterMode(FDungeoneerEditorEdMode::EM_DungeoneerEditorEdModeId);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDungeoneerEditorModule, DungeoneerEditor)