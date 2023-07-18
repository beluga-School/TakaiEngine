#pragma once
#include "Box.h"

class Block : public Box
{
public:
	Block() {
		tag = TagTable::Block;
	};
};
