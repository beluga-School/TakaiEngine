#pragma once
#include "Entity.h"
#include "Box.h"
#include "Vector3.h"

class Block : public Entity
{
public:
	Block() {
		SetTag(TagTable::Block);
		Register();
	};

	void Update();
	void Draw();
	void DrawMaterial();
};
