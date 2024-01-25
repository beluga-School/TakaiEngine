#include "BombSolider.h"
#include "MathF.h"
#include "ObjParticle.h"
#include "Color.h"
#include "Player.h"
#include "TimeManager.h"
#include "InstantDrawer.h"

void BombSolider::Initialize()
{
	Obj3d::Initialize();
	SetModel(ModelManager::GetModel("bombSolider"));
	SetTexture(TextureManager::GetTexture("white"));
	SetOutLineState({ 0,0,0,1 }, 0.05f);

	EncountSphereInitialize();

	//追跡範囲の球の半径(ベースの大きさ+大きさの平均を足す)
	sphereCol.radius = 8 + MathF::Avarage(scale);

	Register();

	actTables.clear();
	SetState(ActTable::None);
}

void BombSolider::Update()
{
	if (isDead)return;

	//近づく->範囲から離れる->爆発
	//近づく->一定距離まで近づく(本体と当たる)->ぷしゅーっていうやつ(attack1)->爆発(attack2)
	//もっといい感じにしたい

	Vector3 standardRotaVec = { -MathF::PIf / 2,0,0 };

	scalingTimer.Update();
	explosionTimer.Update();
	damageTimer.Update();
	partCool.Update();

	if (CheckState(ActTable::None)) {
		
	}
	if (CheckState(ActTable::Tracking)) {
		pVec = Player::Get()->position - position;
		pVec.normalize();
		pVec.y = 0;

		rotation.y = standardRotaVec.Radian(pVec);
		if (pVec.z < 0) {
			//無理やり向きを反対に
			rotation.y *= -1;
		}
		position += pVec * 6.0f * TimeManager::deltaTime;

		//だんだん赤くなる
		color_.y = TEasing::lerp(1.0f, 0.0f,explosionTimer.GetTimeRate());
		color_.z = TEasing::lerp(1.0f, 0.0f, explosionTimer.GetTimeRate());

		///---遷移処理
		//攻撃範囲から外れたら終わり
		//もしくは時間経過で爆発
		if (!Collsions::SphereCollsion(Player::Get()->sphereCol, sphereCol) || 
			explosionTimer.GetEnd())
		{
			SetState(ActTable::Attack1);
			DeleteState(ActTable::Tracking);
			scalingTimer.Start();
		}
	}
	if (CheckState(ActTable::Attack1)) {
		Vector3 end = { def.x * 2, def.y * 2, def.z * 2 };

		scale = TEasing::OutQuad(def, end,scalingTimer.GetTimeRate());

		if (scalingTimer.GetEnd()) {
			SetState(ActTable::Attack2);
			DeleteState(ActTable::Attack1);
			damageTimer.Start();
		}
	}
	if (CheckState(ActTable::Attack2)) {

		if (!partCool.GetRun())
		{
			partCool.Start();
			for (int32_t i = 0; i < 30; i++)
			{
				Color orange = { 1.0f, 0.2f, 0, 1 };
				Color gray = { 0.1f, 0.1f, 0.1f, 1 };
				Color black = { 0.0f, 0.0f, 0.f, 1 };
				Color partColor = gray;

				float maxDistance = 0.0f;

				float randColor = MathF::GetRand(0, 3);
				if (randColor < 1)
				{
					partColor = black;
					maxDistance = 2.0f;
				}
				else if (randColor < 2) {
					partColor = orange;
					maxDistance = 6.0f;
				}
				else
				{
					partColor = gray;
					maxDistance = 10.0f;
				}

				Vector3 randpos = position;

				float partSize = 0.5f;

				ParticleManager::Get()->CreateSphereParticle(randpos, 
					{ partSize,partSize,partSize }, maxDistance,
					{ partColor.f4 .x, partColor.f4.y, partColor.f4.z , partColor.f4.w});
			}
		}

		//丸パーティクル大量発生させる

		//攻撃判定発生
		if (damageTimer.GetEnd()) {
			SetState(ActTable::Dead);
			DeleteState(ActTable::Attack2);
			isDead = true;
		}
	}
	if (CheckState(ActTable::Dead)) {
		
	}

	CollsionUpdate();

	EncountSphereUpdate();

	Obj3d::Update(*Camera::sCamera);
}

void BombSolider::Draw()
{
	if (isDead)return;
	if (CheckState(ActTable::Attack2))return;

	Obj3d::DrawMaterial();
}

void BombSolider::HitEffect()
{
	if (IsDead())return;
}

void BombSolider::Encount()
{
	//ステートがNoneならエンカウントに以降
	if (!CheckState(ActTable::None)) return;

	explosionTimer.Start();

	SetState(ActTable::Tracking);
	DeleteState(ActTable::None);
}