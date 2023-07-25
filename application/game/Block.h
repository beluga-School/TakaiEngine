#pragma once
#include "Entity.h"
#include "Box.h"

class Block : public Entity
{
public:
	Block() {
		taglist.push_back(TagTable::Block);
	};
};
