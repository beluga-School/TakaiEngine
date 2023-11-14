#include "Goal.h"
#include "MathF.h"

void Goal::Initialize()
{
	SetModel(ModelManager::GetModel("GoalPaul"));
	SetTexture(TextureManager::GetTexture("white"));

	goalBlock.Initialize();
	mInitFragPos = true;

	goalBlock.parent = this;

	MODEL;
}

void Goal::Update()
{
	Obj3d::Update(*Camera::sCamera);
	goalBlock.Update();
}

void Goal::Draw()
{
	Obj3d::Draw();
	goalBlock.Draw();
}

void Goal::HitEffect()
{
	goalBlock.HitEffect();
}
