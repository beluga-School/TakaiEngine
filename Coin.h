#pragma once
#include "EventBlock.h"

class Coin final : public EventBlock
{
public:
	Coin() : EventBlock()
	{
		taglist.push_back(TagTable::Coin);
	}

	void Initialize()override;
	void Update()override;
	void Draw()override;
	void HitEffect()override;
};