#pragma once
#include "EventBlock.h"

/*! WarpBlock
	触れるとプレイヤーを別のステージへ飛ばすクラス(Dokan実装後未使用)
*/
class WarpBlock final : public EventBlock
{
public:
	WarpBlock() : EventBlock()
	{
		SetTag(TagTable::WarpBlock);
	}

	void Initialize()override;
	void Update()override;
	void Draw()override;

	void HitEffect()override;
};

