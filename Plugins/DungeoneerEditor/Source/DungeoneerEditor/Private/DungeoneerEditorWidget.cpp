#include "DungeoneerEditorWidget.h"
#include "DungeoneerEditor.h"
#include "DungeoneerEditorEdMode.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SBorder.h"
#include "EditorModeManager.h"
#include "EditorStyleSet.h"

void SDungeoneerEditorWidget::Construct(const FArguments& InArgs)
{
	InnerBorder = SNew(SBorder)
		.BorderImage(FEditorStyle::GetBrush(TEXT("Graph.TitleBackground")))
		.VAlign(VAlign_Fill)
		.Padding(2);
	
	ChildSlot[InnerBorder.ToSharedRef()];
}

FDungeoneerEditorEdMode* SDungeoneerEditorWidget::GetEdMode() const
{
	return (FDungeoneerEditorEdMode*)GLevelEditorModeTools().GetActiveMode(FDungeoneerEditorEdMode::EM_DungeoneerEditorEdModeId);
}

void SDungeoneerEditorWidget::OnCurrentToolChange()
{
	TSharedPtr<SCompoundWidget> toolWidget = GetEdMode()->CurrentTool->GetToolPanelWidget();
	if (toolWidget)
	{
		InnerBorder.ToSharedRef()->SetContent(toolWidget.ToSharedRef());	
	}
	else
	{
		InnerBorder.ToSharedRef()->ClearContent();	
	}
	
}
