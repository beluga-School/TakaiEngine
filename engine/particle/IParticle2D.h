#pragma once
#include "Texture.h"
#include "TEasing.h"
#include "Billboard.h"

class IParticle2D
{
public:
	IParticle2D()
	{
		mActive = true;
		mLifeTimer.Start();
	}

	void Update();

	TEasing::easeTimer mLifeTimer;

	bool mActive = false;

	BillboardY billboard;
};

