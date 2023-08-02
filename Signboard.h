#pragma once
#include "Block.h"

class Signboard : public Block
{
public:
	Signboard() : Block()
	{

	};

	void Initialize();
	void Update();
	void Draw();

	Entity subDrawer;
};

