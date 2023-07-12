#include "GoalBlock.h"
#include "Stage.h"

void GoalBlock::Initialize()
{
	SetModel(ModelManager::GetModel("GoalFrag"));
	SetTexture(TextureManager::GetTexture("white"));

	position = { -1,10,0 };

	//通常オブジェクトと同じアウトラインを付ける
	SetOutLineState({ 1,0,0,1.0f }, 0.05f);
}

void GoalBlock::Update()
{
	goalAnimetionTimer.Update();

	position.y = TEasing::InQuad(10, 4, goalAnimetionTimer.GetTimeRate());

	Obj3d::Update(*Camera::sCamera);
}

void GoalBlock::Draw()
{
	Obj3d::DrawMaterial();
}

void GoalBlock::HitEffect()
{
	Stage::Get()->goalSystem.AnimetionStart();
	if (goalAnimetionTimer.GetStarted() == false)
	{
		goalAnimetionTimer.Start();
	}
}