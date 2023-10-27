#pragma once
#include "EventBlock.h"

/*! Coin
	コインオブジェクト(未使用)
*/
class Coin final : public EventBlock
{
public:
	Coin() : EventBlock()
	{
		SetTag(TagTable::Coin);
	}

	void Initialize()override;
	void Update()override;
	void Draw()override;
	void HitEffect()override;
};