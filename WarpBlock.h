#pragma once
#include "EventBlock.h"

class WarpBlock : public EventBlock
{
	void Initialize()override;
	void Update()override;
	void Draw()override;

	void HitEffect()override;
};

