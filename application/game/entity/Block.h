#pragma once
#include "Entity.h"
#include "Box.h"

class Block : public Entity
{
public:
	Block() {
		SetTag(TagTable::Block);
		Register();
	};

	void Initialize()override;
	void Update()override;
	void Draw()override;
};
