#pragma once
#include "Widgets/SCompoundWidget.h"

class SDungeoneerPaintWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDungeoneerPaintWidget) {}
	SLATE_END_ARGS();

	void Construct(const FArguments& InArgs);
};