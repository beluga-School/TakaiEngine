#include "IParticle2D.h"

void IParticle2D::Update()
{
	mLifeTimer.Update();
	if (mLifeTimer.GetEnd())
	{
		mActive = false;
	}
}
