#pragma once
#include "Enemy.h"

class Skelton : public Enemy
{
public:
	Skelton() :Enemy()
	{

	};

	void Initialize();
	void Update();
	void Draw();

	void HitEffect();

	void Encount();

private:
};

