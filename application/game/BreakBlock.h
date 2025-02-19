#pragma once
#include "Entity.h"
#include "Block.h"

class BreakBlock : public Entity
{
public:
	BreakBlock() : Entity()
	{
		SetTag(TagTable::BreakBlock);
		SetTag(TagTable::Block);
		Register();
	}

	void Initialize()override;
	void Update()override;
	void Draw()override;

	//当たったときの処理
	void HitEffect();

	bool canBreak = false;
};

