#pragma once
#include "Entity.h"
#include "Box.h"

class Block : public Entity
{
public:
	Block() {
		SetTag(TagTable::Block);
		SetTag(TagTable::DitherTransparent);
		Register();
	};

	void Initialize()override;
	void Update()override;
	void Draw()override;
};
