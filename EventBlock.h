#pragma once
#include "Obj.h"

//���[�v������u���b�N
class EventBlock : public Obj3d
{
public:
	void Initialize();

	void Update();
	
	void Draw();

	void HitEffect();

	std::string trigerName = "";

	bool hit = false;
};
