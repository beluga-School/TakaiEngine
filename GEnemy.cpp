#include "GEnemy.h"
#include "Player.h"
#include "TimeManager.h"
#include "ClearDrawScreen.h"
#include "MathF.h"

void GEnemy::Initialize()
{
	Obj3d::Initialize();
	SetModel(ModelManager::GetModel("firewisp"));
	SetTexture(TextureManager::GetTexture("white"));

	hitSphere.Initialize();
	hitSphere.SetModel(ModelManager::GetModel("ICOSphere"));
	hitSphere.SetTexture(TextureManager::GetTexture("white"));
}

void GEnemy::Update()
{
	stayTimer.Update();
	metronomeTimer.Update();
	encountJumpTimer.Update();
	accelerationTimer.Update();

	deadTimer.Update();

	Vector3 standardRotaVec = {MathF::PIf / 2,0,0};

	if (attackState != AttackState::Encount)
	{
		gravity += gravityAdd;
		position -= Vector3(0, gravity, 0);
	}

	switch (attackState)
	{
	case GEnemy::AttackState::None:
		rotation.y = 0;
		break;
	case GEnemy::AttackState::Encount:
		//ジャンプする
		position.y = TEasing::OutQuad(encountJumpS, encountJumpE, encountJumpTimer.GetTimeRate());

		if (encountJumpTimer.GetEnd())
		{
			encountJumpTimer.ReverseStart();
		}
		if (encountJumpTimer.GetReverseEnd())
		{
			attackState = AttackState::Attacking;
			metronomeTimer.Start();
			accelerationTimer.Start();
		}

		break;
	case GEnemy::AttackState::Attacking:
		
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
		rotation.x = TEasing::lerp(-MathF::PIf / 4, MathF::PIf / 4, metronomeTimer.GetTimeRate());

		///---移動処理
		//段々加速したい
		mSpeed = TEasing::InQuad(0.0f, MAX_ACCELERATION, accelerationTimer.GetTimeRate());

		position += pVec * mSpeed * TimeManager::deltaTime;

		///---遷移処理
		//攻撃範囲から外れたら終わり
		if (!Collsions::SphereCollsion(Player::Get()->playerCol, sphereCol))
		{
			attackState = AttackState::Staying;
			stayTimer.Start();
		}

		break;
	case GEnemy::AttackState::Staying:
		
		if (stayTimer.GetEnd())
		{
			attackState = AttackState::None;
			stayTimer.Reset();
		}

		break;
	case GEnemy::AttackState::Dead:
		position.x = TEasing::InQuad(deadEasingS.x, deadEasingE.x, deadTimer.GetTimeRate());
		position.y = TEasing::InQuad(deadEasingS.y, deadEasingE.y, deadTimer.GetTimeRate());
		position.z = TEasing::InQuad(deadEasingS.z, deadEasingE.z, deadTimer.GetTimeRate());

		//yを0に
		rotation.x = 0;
		rotation.y = 0;

		//回転を加算
		rotation.z += deadRoring * TimeManager::deltaTime;

		if (deadTimer.GetEnd())
		{
			isDead = true;
			//パーティクル出したい
		}

		break;
	}

	ColUpdate();

	Obj3d::Update(*Camera::sCamera);
	hitSphere.Update(*Camera::sCamera);
}

void GEnemy::Draw()
{
	BasicObjectPreDraw(PipelineManager::GetPipeLine("Toon"));
	Obj3d::DrawMaterial();

	/*BasicObjectPreDraw(PipelineManager::GetPipeLine("WireFrame"));
	hitSphere.Draw();*/

	BasicObjectPreDraw(PipelineManager::GetPipeLine("Toon"));

}

void GEnemy::HitEffect()
{
	//すでに死亡済みならスキップ
	if (attackState == AttackState::Dead)return;
	attackState = AttackState::Dead;
	
	//死亡時のプレイヤーが向いていた方向を保存
	deadDirection = Player::Get()->matWorld.ExtractAxisZ();
	
	//斜め上に吹っ飛んでいくように
	float rand = MathF::GetRand(6.0f, 8.0f);
	deadDirection.y = MathF::PIf / rand;

	deadEasingS = position;

	//正面に向かって吹っ飛ばされるように終点を設定
	deadEasingE = position + deadDirection * 7.0f;

	deadTimer.Start();
}

void GEnemy::Encount()
{
	//ステートがNoneならエンカウントに以降
	if (attackState != AttackState::None)return;
	attackState = AttackState::Encount;
	encountJumpTimer.Start();
	encountJumpS = position.y;
	encountJumpE = position.y + 2.0f;
}

void GEnemy::ColUpdate()
{
	sphereCol.center = position;
	//追跡範囲の球の半径
	sphereCol.radius = 8;

	hitSphere.position = sphereCol.center;
	hitSphere.scale = { sphereCol.radius,sphereCol.radius,sphereCol.radius };
}
