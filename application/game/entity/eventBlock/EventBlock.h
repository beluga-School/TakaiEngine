#pragma once
#include "Obj.h"
#include "Block.h"

class EventBlock : public Block
{
public:
	EventBlock() :Block()
	{

	};

	virtual void Initialize() = 0;

	virtual void Update() = 0;
	
	virtual void Draw() = 0;

	virtual void HitEffect() = 0;

	virtual ~EventBlock(){};

	std::string trigerName = "";

	bool hit = false;
};
