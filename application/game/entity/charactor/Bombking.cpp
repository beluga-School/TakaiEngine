#include "Bombking.h"
#include "Player.h"
#include "MathF.h"
#include "TimeManager.h"

void Bombking::Initialize()
{
	Obj3d::Initialize();
	SetModel(ModelManager::GetModel("bombking"));
	SetTexture(TextureManager::GetTexture("white"));
	SetOutLineState({ 0,0,0,1 }, 0.05f);
	
	EncountSphereInitialize();

	Register();
}

void Bombking::Update()
{
	/*if (state != BossState::Encount)
	{
		gravity += gravityAdd;
		position -= Vector3(0, gravity, 0);
	}*/

	switch (mActTable)
	{
	case ActTable::None:
		break;
	case ActTable::Encount:
		mActTable = ActTable::Tracking;
		break;
	case ActTable::Tracking:

		Tracking();

		break;
	case ActTable::Attack1:
		break;
	case ActTable::Attack2:
		break;
	case ActTable::Staying:
		break;
	}

	//ずらした分を加算する
	box.CreateCol(position + saveColCenter, box.scale);
	box.ColDrawerUpdate(position + saveColCenter, box.scale);

	CollsionUpdate();

	EncountSphereUpdate();

	Obj3d::Update(*Camera::sCamera);
}

void Bombking::Draw()
{
	Obj3d::DrawMaterial();
}

void Bombking::HitEffect()
{
}

void Bombking::Encount()
{
	mActTable = ActTable::Encount;
}

void Bombking::Tracking()
{
	pVec = Enemy::TargetVector(*Player::Get());

	Enemy::TargetTurn(pVec);

	position += pVec * mSpeed * TimeManager::deltaTime;
}
