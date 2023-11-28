#include "Bombking.h"
#include "Player.h"
#include "MathF.h"
#include "TimeManager.h"
#include "ObjParticle.h"

void Bombking::Initialize()
{
	Obj3d::Initialize();
	SetModel(ModelManager::GetModel("bombking"));
	SetTexture(TextureManager::GetTexture("white"));
	SetOutLineState({ 0,0,0,1 }, 0.05f);
	
	EncountSphereInitialize();

	Register();

	//現在HPを設定 エディタ側からの指定に対応したい
	SetMaxHP(3);
}

void Bombking::Update()
{
	damageTimer.Update();
	reboundTimer.Update();

	//ダメージ受けたときにこの大きさへと変化する
	Vector3 damageScale = { initScale.x + 1.0f,initScale.y - 0.5f, initScale.z + 1.0f };

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
	case ActTable::Damage:
		if (damageTimer.GetEnd()) {
			if (hp.mCurrent > 0)
			{
				if(!reboundTimer.GetStarted())reboundTimer.Start();
			}
			else
			{
				mActTable = ActTable::Dead;
			}
		}
		if (reboundTimer.GetEnd()) {
			mActTable = ActTable::None;
			reboundTimer.Reset();
			damageTimer.Reset();
		}

		if (damageTimer.GetRun())
		{
			scale.x = TEasing::OutElastic(initScale.x, damageScale.x, damageTimer.GetTimeRate());
			scale.y = TEasing::OutElastic(initScale.y, damageScale.y, damageTimer.GetTimeRate());
			scale.z = TEasing::OutElastic(initScale.z, damageScale.z, damageTimer.GetTimeRate());
		}

		if (reboundTimer.GetRun())
		{
			scale.x = TEasing::OutElastic(damageScale.x,initScale.x, reboundTimer.GetTimeRate());
			scale.y = TEasing::OutElastic(damageScale.y, initScale.y, reboundTimer.GetTimeRate());
			scale.z = TEasing::OutElastic(damageScale.z, initScale.z, reboundTimer.GetTimeRate());
		}

		break;
	case ActTable::Dead:
		isDead = true;
		//パーティクル出す
		for (int32_t i = 0; i < 50; i++)
		{
			ParticleManager::Get()->CreateCubeParticle(position,
				{ 1.0f,1.0f,1.0f }, 10, { 0,1,0,0.5f });
		}
		break;
	}

	//ずらした分を加算する
	box.CreateCol(position + saveColCenter, box.scale, rotation);
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
	if (damageTimer.GetRun())return;
	//食らったらちょっと潰れる
	if(!damageTimer.GetStarted())damageTimer.Start();
	mActTable = ActTable::Damage;

	hp.mCurrent -= 1;
}

void Bombking::Encount()
{
	if (mActTable != ActTable::Damage)
	{
		mActTable = ActTable::Encount;
	}
}

void Bombking::Tracking()
{
	pVec = Enemy::TargetVector(*Player::Get());

	Enemy::TargetTurn(pVec);

	position += pVec * mSpeed * TimeManager::deltaTime;
}
