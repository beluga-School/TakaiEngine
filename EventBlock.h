#pragma once
#include "Obj.h"

//ワープさせるブロック
class EventBlock : public Obj3d
{
public:
	virtual void Initialize() = 0;

	virtual void Update() = 0;
	
	virtual void Draw() = 0;

	virtual void HitEffect() = 0;

	virtual ~EventBlock(){};

	std::string trigerName = "";

	bool hit = false;
};
