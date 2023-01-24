#include "DungeoneerEdMode.h"
#include "DungeoneerEdModeToolkit.h"
#include "Toolkits/ToolkitManager.h"
#include "EditorModeManager.h"

const FEditorModeID FDungeoneerEdMode::EM_DungeoneerEdModeId = TEXT("EM_DungeoneerEdMode");

FDungeoneerEdMode::FDungeoneerEdMode()
{

}

FDungeoneerEdMode::~FDungeoneerEdMode()
{

}

void FDungeoneerEdMode::Enter()
{
	FEdMode::Enter();

	if (!Toolkit.IsValid() && UsesToolkits())
	{
		Toolkit = MakeShareable(new FDungeoneerEdModeToolkit);
		Toolkit->Init(Owner->GetToolkitHost());
	}
}

void FDungeoneerEdMode::Exit()
{
	if (Toolkit.IsValid())
	{
		FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
		Toolkit.Reset();
	}

	// Call base Exit method to ensure proper cleanup
	FEdMode::Exit();
}

void FDungeoneerEdMode::Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI)
{
	FEdMode::Render(View, Viewport, PDI);
}

bool FDungeoneerEdMode::UsesToolkits() const
{
	return true;
}




