#pragma once
#include "DungeoneerTool.h"

class FDungeoneerPaintTool final : public FDungeoneerTool
{
public:
	FDungeoneerPaintTool(FDungeoneerEditorEdMode* _Mode) : FDungeoneerTool(_Mode) {}

	virtual FName GetToolName() override { return "TOOL_PAINT"; };

	virtual TSharedPtr<SCompoundWidget> GenerateToolPanel() override;
	
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override {};
};