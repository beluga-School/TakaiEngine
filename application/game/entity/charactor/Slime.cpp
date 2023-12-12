#include "Slime.h"
#include "Player.h"
#include "TimeManager.h"
#include "ClearDrawScreen.h"
#include "MathF.h"
#include "ObjParticle.h"

void Slime::Initialize()
{
	Obj3d::Initialize();
	SetModel(ModelManager::GetModel("slime"));
	SetTexture(TextureManager::GetTexture("white"));
	SetOutLineState({0,0,0,1},0.05f);

	EncountSphereInitialize();

	//追跡範囲の球の半径(ベースの大きさ+大きさの平均を足す)
	sphereCol.radius = 8 + MathF::Avarage(scale);

	Register();

	actTables.clear();
	SetState(ActTable::None);
}

void Slime::Update()
{
	stayTimer.Update();
	metronomeTimer.Update();
	encountJumpTimer.Update();
	accelerationTimer.Update();

	deadTimer.Update();

	Vector3 standardRotaVec = {MathF::PIf / 2,0,0};

	if (CheckState(ActTable::None)) {
		rotation.y = 0;
	}
	if (CheckState(ActTable::Encount)) {
		if (encountJumpTimer.GetEnd())
		{
			encountJumpTimer.ReverseStart();
		}
		if (encountJumpTimer.GetReverseEnd())
		{
			SetState(ActTable::Tracking);
			DeleteState(ActTable::Encount);
			metronomeTimer.Start();
			accelerationTimer.Start();
		}
	}
	if (CheckState(ActTable::Tracking)) {
		///---見た目処理
		//追いかける方向へ向きを変える
		pVec = Player::Get()->position - position;
		pVec.normalize();
		pVec.y = 0;

		rotation.y = standardRotaVec.Radian(pVec);
		if (pVec.z > 0) {
			//無理やり向きを反対に
			rotation.y *= -1;
		}

		//横揺れする
		if (metronomeTimer.GetEnd())
		{
			metronomeTimer.ReverseStart();
		}
		if (metronomeTimer.GetReverseEnd())
		{
			metronomeTimer.Start();
		}
		scale.y = TEasing::lerp(initScale.y - 0.2f, initScale.y + 0.2f, metronomeTimer.GetTimeRate());

		///---移動処理
		//段々加速したい
		mSpeed = TEasing::InQuad(0.0f, MAX_ACCELERATION, accelerationTimer.GetTimeRate());

		position += pVec * mSpeed * TimeManager::deltaTime;

		///---遷移処理
		//攻撃範囲から外れたら終わり
		if (!Collsions::SphereCollsion(Player::Get()->mEncountCol, sphereCol))
		{
			SetState(ActTable::Staying);
			DeleteState(ActTable::Tracking);
			stayTimer.Start();
		}
	}
	if (CheckState(ActTable::Staying)) {
		if (stayTimer.GetEnd())
		{
			SetState(ActTable::None);
			DeleteState(ActTable::Staying);
			stayTimer.Reset();
		}
	}
	if (CheckState(ActTable::Dead)) {
		//縦はつぶす
		scale.y = TEasing::OutQuad(deadEasingS.y, deadEasingE.y, deadTimer.GetTimeRate());

		//横は伸ばす
		scale.x = TEasing::OutQuad(deadEasingS.x, deadEasingE.x, deadTimer.GetTimeRate());
		scale.z = TEasing::OutQuad(deadEasingS.z, deadEasingE.z, deadTimer.GetTimeRate());

		//yを0に
		rotation.x = 0;
		rotation.y = 0;

		if (deadTimer.GetEnd())
		{
			isDead = true;
			//パーティクル出す
			for (int32_t i = 0; i < 50; i++)
			{
				ParticleManager::Get()->CreateCubeParticle(position,
					{ 1.0f,1.0f,1.0f }, 10, { 0,1,0,0.5f });
			}
		}

	}
	//ずらした分を加算する
	box.CreateCol(position + saveColCenter, box.scale, rotation);
	box.ColDrawerUpdate(position + saveColCenter, box.scale);

	CollsionUpdate();

	EncountSphereUpdate();

	Obj3d::Update(*Camera::sCamera);
}

void Slime::Draw()
{
	Obj3d::DrawMaterial();
}

void Slime::HitEffect()
{
	//すでに死亡済みならスキップ
	if (IsDead())return;
	SetState(ActTable::Dead);

	//潰れるようにしたい
	deadTimer.Start();

	deadEasingS = scale;
	deadEasingE = {scale.x * 1.5f,0.1f ,scale.z * 1.5f};
}

void Slime::Encount()
{
	//ステートがNoneならエンカウントに以降
	if (!CheckState(ActTable::None)) return;

	SetState(ActTable::Encount);
	DeleteState(ActTable::None);

	encountJumpTimer.Start();
	encountJumpS = position.y;
	encountJumpE = position.y + 0.2f;
}