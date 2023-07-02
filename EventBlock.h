#pragma once
#include "Obj.h"

//ワープさせるブロック
class EventBlock : public Obj3d
{
public:
	void Initialize();

	void Update();
	
	void Draw();

	virtual void HitEffect() = 0;

	std::string trigerName = "";

	bool hit = false;
};
