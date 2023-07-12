#pragma once
#include "EventBlock.h"

class Star : public EventBlock
{
public:
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void HitEffect()override;
};

