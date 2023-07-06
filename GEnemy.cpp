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

	targetMark.Initialize();
	targetMark.SetModel(ModelManager::GetModel("targetMark"));
	targetMark.SetTexture(TextureManager::GetTexture("white"));
}

void GEnemy::Update()
{
	stayTimer.Update();
	metronomeTimer.Update();
	encountJumpTimer.Update();
	attakingTimer.Update();
	markRotaTimer.Update();

	Vector3 standardRotaVec = {MathF::PIf / 2,0,0};

	switch (state)
	{
	case GEnemy::State::None:
		rotation.y = 0;
		break;
	case GEnemy::State::Encount:
		//ジャンプする
		position.y = TEasing::OutQuad(encountJumpS, encountJumpE, encountJumpTimer.GetTimeRate());

		if (encountJumpTimer.GetEnd())
		{
			encountJumpTimer.ReverseStart();
		}
		if (encountJumpTimer.GetReverseEnd())
		{
			state = State::Attacking;
			metronomeTimer.Start();
			attakingTimer.Start();

			{
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
			}

			//次の位置決定
			attackPosS = position;
			//プレイヤーの位置 + その少し奥(移動方向 * 移動させたい距離) = 終点を出す
			attackPosE = Player::Get()->position + pVec * attackDistance;

			//最終位置にターゲットマークを出す
			targetMark.position = attackPosE;
			targetMark.position.y = Player::Get()->GetFeet();

			//加速を元に戻す
			mSpeed = 5.0f;
		}

		break;
	case GEnemy::State::Attacking:
		
		//{
		//	///---見た目処理
		//	//追いかける方向へ向きを変える
		//	pVec = Player::Get()->position - position;
		//	pVec.normalize();
		//	pVec.y = 0;
	
		//	rotation.y = standardRotaVec.Radian(pVec);
		//	if (pVec.z > 0) {
		//		//無理やり向きを反対に
		//		rotation.y *= -1;
		//	}
		//}

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

		//マークを回転
		targetMark.rotation.y = TEasing::lerp(0, MathF::PIf * 2, markRotaTimer.GetTimeRate());

		///---移動処理
		//段々加速したい
		mSpeed += acceleration;
		mSpeed = Util::Clamp(mSpeed, 0.0f, MAX_ACCELERATION);

		position += pVec * mSpeed * TimeManager::deltaTime;

		//position.x = TEasing::InQuad(attackPosS.x,attackPosE.x,attakingTimer.GetTimeRate());
		//position.z = TEasing::InQuad(attackPosS.z,attackPosE.z,attakingTimer.GetTimeRate());

		///---遷移処理
		//突進が終わったら終わり
		//目標の地点に到達するか、壁にぶつかったら終わり
		//いったん目標地点だけで作る
		if (attakingTimer.GetEnd())
		{
			state = State::Staying;
			attakingTimer.Reset();
			stayTimer.Start();
		}

		break;
	case GEnemy::State::Staying:
		
		if (stayTimer.GetEnd())
		{
			state = State::None;
			stayTimer.Reset();
		}

		break;
	}

	ColUpdate();

	Obj3d::Update(*Camera::sCamera);
	hitSphere.Update(*Camera::sCamera);
	targetMark.Update(*Camera::sCamera);
}

void GEnemy::Draw()
{
	BasicObjectPreDraw(PipelineManager::GetPipeLine("Toon"));
	Obj3d::DrawMaterial();
	
	if (state == State::Attacking)
	{
		targetMark.DrawMaterial();
	}

	BasicObjectPreDraw(PipelineManager::GetPipeLine("WireFrame"));
	hitSphere.Draw();

	BasicObjectPreDraw(PipelineManager::GetPipeLine("Toon"));

}

void GEnemy::HitEffect()
{
	isDead = true;
}

void GEnemy::Encount()
{
	//ステートがNoneならエンカウントに以降
	if (state != State::None)return;
	state = State::Encount;
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
