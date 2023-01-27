#pragma once

#include "CoreMinimal.h"
#include "Toolkits/BaseToolkit.h"
#include "Dungeon.h"
#include "IStructureDetailsView.h"
#include "IDetailsView.h"

class FDungeoneerEdModeToolkit : public FModeToolkit
{
public:

	FDungeoneerEdModeToolkit();
	
	/** FModeToolkit interface */
	virtual void Init(const TSharedPtr<IToolkitHost>& InitToolkitHost) override;

	/** IToolkit interface */
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual class FEdMode* GetEditorMode() const override;
	virtual TSharedPtr<class SWidget> GetInlineContent() const override
	{
		return ToolkitWidget;
	}


	void SelectTileSegment(ADungeon* LevelDungeon, FDungeonTileSegmentHash selected);
	
private:

	TSharedPtr<IStructureDetailsView> Details;
	TSharedPtr<SWidget> ToolkitWidget;
};
