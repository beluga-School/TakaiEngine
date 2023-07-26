#pragma once
#include "EventBlock.h"

class WarpBlock final : public EventBlock
{
public:
	WarpBlock() : EventBlock()
	{
		taglist.push_back(TagTable::WarpBlock);
	}

	void Initialize()override;
	void Update()override;
	void Draw()override;

	void HitEffect()override;
};

