#include "DungeoneerEditorWidget.h"
#include "DungeoneerEditor.h"
#include "DungeoneerEditorEdMode.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SBorder.h"
#include "EditorModeManager.h"
#include "EditorStyleSet.h"

void SDungeoneerEditorWidget::Construct(const FArguments& InArgs)
{
	TemplateWidget = SNew(SDungeoneerTemplateWidget);
	
	ChildSlot
	[
			SNew(SBorder)
				.BorderImage(FEditorStyle::GetBrush(TEXT("Graph.TitleBackground")))
				.VAlign(VAlign_Fill)
				.Padding(2)
				[
					TemplateWidget.ToSharedRef()
				]
	];

}