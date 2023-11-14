#include "EnemyDokan.h"
#include "EnemyManager.h"
#include "GEnemy.h"

void EnemyDokan::LoadResource()
{
	ModelManager::Get()->LoadModel("red_dokan", "red_dokan", true);
}

void EnemyDokan::Initialize()
{
	SetModel(ModelManager::GetModel("red_dokan"));
}

void EnemyDokan::Update()
{
	popCool.Update();
	if (!popCool.GetRun())
	{
		popCool.Start();
		EnemyManager::Get()->PopEnemy<Slime>(position,{0,0,0},{1,1,1});
	}
	Obj3d::Update(*Camera::sCamera);
}

void EnemyDokan::Draw()
{
	Obj3d::DrawMaterial();
}

void EnemyDokan::HitEffect()
{
	
}
