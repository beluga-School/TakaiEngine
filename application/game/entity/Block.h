#pragma once
#include "Entity.h"
#include "Box.h"

struct ConstBufferPlayerPos {
	Vector3 value;
};

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

	ConstBuffer<ConstBufferPlayerPos> constBufferPlayerPos;
};
