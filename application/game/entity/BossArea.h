#pragma once
#include "Entity.h"
#include "Block.h"

class BossArea : public Entity
{
public:
	BossArea() : Entity()
	{
		SetTag(TagTable::BossArea);
	}

	void Create();

	void Initialize()override;
	void Update()override;
	void Draw()override;

	std::vector<Block> bossAreaParts;
};

