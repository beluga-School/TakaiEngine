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
	colDrawer.Initialize();
	colDrawer.SetModel(ModelManager::GetModel("BlankCube"));
	colDrawer.SetTexture(TextureManager::GetTexture("white"));

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

	switch (state)
	{
	case Bombking::BossState::None:
		break;
	case Bombking::BossState::Encount:
		state = BossState::Tracking;
		break;
	case Bombking::BossState::Tracking:

		Tracking();

		break;
	case Bombking::BossState::Punch:
		break;
	case Bombking::BossState::HipDrop:
		break;
	case Bombking::BossState::CoolTime:
		break;
	}


	colDrawer.position = position;
	colDrawer.scale = { 5,5,5 };

	colDrawer.position.y += colDrawer.scale.y / 2;
	colDrawer.Update(*Camera::sCamera);
	
	box.CreateCol(colDrawer.position, colDrawer.scale);

	CollsionUpdate();

	EncountSphereUpdate();

	Obj3d::Update(*Camera::sCamera);
}

void Bombking::Draw()
{
	Obj3d::DrawMaterial();
	colDrawer.Draw();
}

void Bombking::HitEffect()
{
}

void Bombking::Encount()
{
	state = BossState::Encount;
}

void Bombking::Tracking()
{
	pVec = Enemy::TargetVector(*Player::Get());

	Enemy::TargetTurn(*Player::Get(),pVec);

	position += pVec * mSpeed * TimeManager::deltaTime;
}
