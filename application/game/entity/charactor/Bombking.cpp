#include "Bombking.h"
#include "Player.h"
#include "MathF.h"
#include "TimeManager.h"
#include "ObjParticle.h"
#include "ClearDrawScreen.h"
#include "CollideManager.h"

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

	throwBox.Initialize();

	target = nullptr;

	mActTable = ActTable::Encount;
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
		throwTimer.Update();
		throwInterval.Update();

		//ターゲットに中身があるなら
		if (target != nullptr)
		{
			if (throwInterval.GetEnd()) {
				if (!throwTimer.GetStarted()) {
					throwTimer.Start();
				}
			}
			target->position = Vector3::Spline(inters, throwTimer.GetTimeRate());
			
			if (throwTimer.GetEnd()) {
				target->SetNoGravity(false);
				target->SetNoCollsion(false);
				target = nullptr;
				mActTable = ActTable::Encount;
			}
		}

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
		}

		if (reboundTimer.GetEnd()) {
			mActTable = ActTable::Encount;
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

	//死亡処理への遷移
	if (damageTimer.GetEnd())
	{
		if (hp.mCurrent <= 0)
		{
			mActTable = ActTable::Dead;
		}
	}

	throwBox.position = position + (Enemy::TargetVector(*Player::Get()) * 5);
	throwBox.scale = { 5,5,5 };
	throwBox.Update(*Camera::sCamera);

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

	BasicObjectPreDraw("WireFrame");
	throwBox.Draw();
}

void Bombking::HitEffect()
{
	if (mActTable == ActTable::Damage){
		return;
	}
	//食らったらちょっと潰れる
	if (!damageTimer.GetStarted()) {
		damageTimer.Start();
	}

	mActTable = ActTable::Damage;

	hp.mCurrent -= 1;
}

void Bombking::Encount()
{
	if ((mActTable != ActTable::Damage && mActTable != ActTable::Attack1))
	{
		mActTable = ActTable::Encount;
	}
}

bool Bombking::ThrowBoxHit(const Mob& mob)
{
	return Collsions::BoxColAABB(mob.box,throwBox);
}

void Bombking::Throw(Mob& mob)
{
	if (target != nullptr)return;
	//入った時点でのターゲットベクターを保持し、その方向へとぶっ飛ばす
	//スプライン的な放物線か、円運動を半周させるか
	//スプラインがいいか
	Vector3 targetVec = Enemy::TargetVector(*Player::Get());

	//最終位置と中点を割り出し

	//初期値店は腕の当たり
	Vector3 startPos = position + (targetVec * 3) + Vector3(0, 4, 0);
	Vector3 interPos = startPos + (targetVec * 20) + Vector3(0,5,0);
	//最終地点はちゃんと地面にしてあげたいが、とりあえず遠くに投げ飛ばす
	Vector3 endPos = startPos + (targetVec * 30);

	target = &mob;
	target->SetNoGravity(true);
	target->SetNoCollsion(true);

	endPos.y = target->position.y;

	inters.clear();
	inters.push_back(startPos);
	inters.push_back(interPos);
	inters.push_back(endPos);

	throwInterval.Start();
	throwTimer.Reset();

	mActTable = ActTable::Attack1;

	//endTargetCircle.position = endPos;
}

void Bombking::Tracking()
{
	pVec = Enemy::TargetVector(*Player::Get());

	Enemy::TargetTurn(pVec);

	position += pVec * mSpeed * TimeManager::deltaTime;
}
