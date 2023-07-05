#include "Player.h"
#include "Input.h"
#include "TimeManager.h"
#include "Stage.h"
#include "ImguiManager.h"
#include "MathF.h"
#include "PlayerCamera.h"
#include "EnemyManager.h"
#include "ClearDrawScreen.h"

using namespace Input;

enum class CheckDirection
{
	CD_UP,
	CD_DOWN,
	CD_CENTER,
	CD_BACK,
	CD_LEFT,
	CD_RIGHT,
};

bool CheckDirections(const Cube& cubeCol, const Cube& blockCol,const CheckDirection& CD)
{
	switch (CD)
	{
	case CheckDirection::CD_UP:
		return cubeCol.position.y > blockCol.position.y + blockCol.scale.y * 0.5f;
		break;
	case CheckDirection::CD_DOWN:
		return cubeCol.position.y < blockCol.position.y - blockCol.scale.y * 0.5f;
		break;
	case CheckDirection::CD_CENTER:
		return cubeCol.position.z > blockCol.position.z + blockCol.scale.z * 0.5f;;
		break;
	case CheckDirection::CD_BACK:
		return cubeCol.position.z < blockCol.position.z - blockCol.scale.z * 0.5f;
		break;
	case CheckDirection::CD_LEFT:
		return cubeCol.position.x > blockCol.position.x + blockCol.scale.x * 0.5f;
		break;
	case CheckDirection::CD_RIGHT:
		return cubeCol.position.x < blockCol.position.x - blockCol.scale.x * 0.5f;
		break;
	}
	return false;
}

void Player::Initialize()
{
	Obj3d::Initialize();
	SetModel(ModelManager::GetModel("beetle"));

	SetOutLineState({ 0.1f,0.1f,0.1f }, 0.05f);
}

void Player::Update()
{
	//移動地を初期化
	moveValue = { 0,0,0 };

	//ベクトルを保存
	mCenterVec = matWorld.ExtractAxisZ();
	mSideVec = matWorld.ExtractAxisX();


	//攻撃更新
	attackingTimer.Update();
	attackCoolTimer.Update();
	attackingMotionTimer.Update();

	//入力
	switch (attackState)
	{
	case Player::AttackState::None:
		//攻撃していない状態で入力があったら
		if (Mouse::Triggered(Click::LEFT))
		{
			//攻撃状態に遷移
			attackState = AttackState::Attacking;
			attackingTimer.Start();
		
			//ここで正面ベクトルを保存
			mRolingVec = mCenterVec;

			//ローリング数が適用されるように最大時間を割る
			attackingMotionTimer.mMaxTime = attackingTimer.mMaxTime;
			//attackingMotionTimer.mMaxTime = attackingTimer.mMaxTime;
		}
		break;
	case Player::AttackState::Attacking:
		
		if (attackingMotionTimer.GetRun() == false)
		{
			attackingMotionTimer.Start();
		}

		//正面ベクトルの方向に進める
		moveValue += mRolingVec * mRolingSpeed * TimeManager::deltaTime;
		rotation.x = TEasing::InQuad(-MathF::PIf * 2.f,MathF::PIf * 2.f, attackingMotionTimer.GetTimeRate());

		if (attackingTimer.GetEnd())
		{
			attackState = AttackState::CoolTime;
			attackingTimer.Reset();
			attackCoolTimer.Start();
			
			rotation.x = 0;
		}
		break;
	case Player::AttackState::CoolTime:
		if (attackCoolTimer.GetEnd())
		{
			attackState = AttackState::None;
			attackCoolTimer.Reset();
		}
		break;
	}

	//横移動更新
	if (attackState != AttackState::Attacking)
	{
		SideMoveUpdate();
	}
	
	//縦移動更新(重力落下含む)
	JumpUpdate();

	//当たり判定更新
	ColUpdate();

	//本加算
	position.y = preMove.y;
	position += moveValue;

	//回転更新
	if (attackState != AttackState::Attacking)
	{
		RotaUpdate();
	}

	//更新
	Obj3d::Update(*Camera::sCamera);
}

void Player::Draw()
{
	BasicObjectPreDraw(PipelineManager::GetPipeLine("OutLine"), false);
	Obj3d::DrawOutLine();

	BasicObjectPreDraw(PipelineManager::GetPipeLine("Toon"));
	Obj3d::DrawMaterial();
}

void Player::Reset()
{
	gravity = 0;
}

void Player::SideMoveUpdate()
{
	//移動量を取得、加算
	if (Input::Pad::CheckConnectPad())
	{
		moveValue += mCenterVec * Pad::GetLStickMove().y * mSpeed * TimeManager::deltaTime;
		moveValue += mSideVec * Pad::GetLStickMove().x * mSpeed * TimeManager::deltaTime;
	}

	if (Input::Keyboard::PushKey(DIK_W))
	{
		moveValue += mCenterVec * mSpeed * TimeManager::deltaTime;
	}
	if (Input::Keyboard::PushKey(DIK_S))
	{
		moveValue -= mCenterVec * mSpeed * TimeManager::deltaTime;
	}
	if (Input::Keyboard::PushKey(DIK_D))
	{
		moveValue += mSideVec * mSpeed * TimeManager::deltaTime;
	}
	if (Input::Keyboard::PushKey(DIK_A))
	{
		moveValue -= mSideVec * mSpeed * TimeManager::deltaTime;
	}
}

void Player::JumpUpdate()
{
	jumpManageTimer.Update();
	stayManageTimer.Update();

	switch (jumpState)
	{
	case Player::JumpState::None:
		//ジャンプしていない状態で入力があったらジャンプ
		if (Input::Keyboard::TriggerKey(DIK_SPACE) ||
			Pad::TriggerPadButton(PadButton::A))
		{
			//値を指定
			upJumpS = position.y;
			upJumpE = position.y + jumpPower;

			jumpManageTimer.Start();
			jumpState = JumpState::Up;

			//重力を無効化
			gravity = 0;
		}

		//hitListの中で、最も高い位置にあるオブジェクトより自身の座標が高かったら
		if (position.y > hitCubeMaxY)
		{
			//重力落下させる
			gravity += gravityAdd;
			preMove.y -= gravity * TimeManager::deltaTime;
		}
		//hitListオブジェクトの中で、最も高い位置にあるオブジェクトに自身が当たっているなら
		else
		{
			gravity = 0;
			preMove.y = hitCubeMaxY;
		}

		break;
	case Player::JumpState::Up:
		//イージングで上昇
		preMove.y = TEasing::OutQuad(upJumpS, upJumpE, jumpManageTimer.GetTimeRate());

		//時間が終わったらステートを次の状態に遷移
		if (jumpManageTimer.GetEnd())
		{
			jumpState = JumpState::Staying;
			jumpManageTimer.Reset();
			stayManageTimer.Start();
		}

		break;
	case Player::JumpState::Staying:
		if (stayManageTimer.GetEnd())
		{
			jumpState = JumpState::None;
			stayManageTimer.Reset();
		}

		break;
	}
}

GUI checkGUI("hitCheck");

void Player::ColUpdate()
{
	//当たり判定
	Cube pCol;
	pCol.position = position;
	pCol.scale = scale;

	pCol.position.y = preMove.y;

	pCol.position += moveValue;

	for (auto& bCol : Stage::Get()->mColCubes)
	{
		//そのオブジェクトより
		//上にいるか
		bool up = CheckDirections(pCol, bCol, CheckDirection::CD_UP);
		//下にいるか
		bool down = CheckDirections(pCol, bCol, CheckDirection::CD_DOWN);
		//前にいるか
		bool center = CheckDirections(pCol, bCol, CheckDirection::CD_CENTER);
		//後ろにいるか
		bool back = CheckDirections(pCol, bCol, CheckDirection::CD_BACK);
		//左にいるか
		bool left = CheckDirections(pCol, bCol, CheckDirection::CD_LEFT);
		//右にいるか
		bool right = CheckDirections(pCol, bCol, CheckDirection::CD_RIGHT);

		//上面の当たり判定
		if (up)
		{
			//直線状で見たときに下にあるオブジェクトがあれば
			Cube rayCube;
			rayCube.position = pCol.position;
			rayCube.scale = pCol.scale;
			//スケールをめっちゃ引き延ばす
			rayCube.scale.y = 100;

			//当たったなら
			bool cubeCol = Collsions::CubeCollision(rayCube, bCol);

			if (cubeCol)
			{
				//リストに入れる
				
				//このリストをいちいち消すのではなく、
				//当たり判定をとり、同じ要素が入っていないなら入れて
				//当たり判定が外れたときに、その要素を消す
				UniqueObjectPushBack(hitListY,bCol);
			}
			else
			{
				UniqueObjectErase(hitListY,bCol);
			}
		}

		//下面の当たり判定
		if (down)
		{
			
		}

		//左右の当たり判定
		if (up == false)
		{
			//左右も別の当たり判定リスト作って、同じ手法で取れそう？
			if (Collsions::CubeCollision(pCol, bCol))
			{
				//横方向を少し大きくして、当たり判定を取ったオブジェクトと当たっているなら
				if (right)
				{
					moveValue.x = 0;
				}
				if (left)
				{
					moveValue.x = 0;
				}
				if (back)
				{
					moveValue.z = 0;
				}
				if (center)
				{
					moveValue.z = 0;
				}
			}
			//if (center)
			//{
			//	Cube sideCube;
			//	sideCube.position = pCol.position;
			//	//横方向の大きさを2倍に
			//	sideCube.scale.z = pCol.scale.z * 2.f;
			//	if (Collsions::CubeCollision(sideCube, bCol))
			//	{
			//		UniqueObjectPushBack(hitListX, bCol);
			//	}
			//	else
			//	{
			//		UniqueObjectErase(hitListX, bCol);

			//	}
			//	moveValue.z = 0;
			//}
		}
	}

	//Y軸の判定
	float preY = -114514.f;
	float maxY = 0;

	hitCubeMaxY = preY;

	for (auto& hit : hitListY)
	{
		maxY = hit.position.y;
		//初期値でなく、前の値より高い位置にあるなら
		if (maxY >= preY)
		{
			//終点位置を更新
			//少しだけ浮かせて、ブロックの切れ目に引っかからないように
			hitCubeMaxY = hit.position.y + hit.scale.y / 2 + pCol.scale.y / 2 + 0.01f;
		}
		preY = hit.position.y;
	}

	////X軸の判定
	//for (auto &hit : hitListX)
	//{
	//	maxY = hit.position.y;
	//	//初期値でなく、前の値より高い位置にあるなら
	//	if (maxY >= preY)
	//	{
	//		//終点位置を更新
	//		//少しだけ浮かせて、ブロックの切れ目に引っかからないように
	//		hitCubeMaxY = hit.position.y + hit.scale.y / 2 + pCol.scale.y / 2 + 0.01f;
	//	}
	//	preY = hit.position.y;
	//}

	for (auto& bColevent : Stage::Get()->mEventObjects)
	{
		Cube eCol;
		eCol.position = bColevent->position;
		
		//なんか判定が小さかったので2倍に そしたらぴったりだったので、どっかで半分に
		//する処理が挟まってる
		eCol.scale = bColevent->scale * 2;
		
		if (Collsions::CubeCollision(eCol, pCol))
		{
			bColevent->HitEffect();

			break;
		}
	}

	for (auto& enemy : EnemyManager::Get()->enemyList)
	{
		Cube enemyCol;
		enemyCol.position = enemy->position;
		enemyCol.scale = enemy->scale;
		if (Collsions::CubeCollision(enemyCol, pCol))
		{
			enemy->HitEffect();
			break;
		}
	}

}

void Player::RotaUpdate()
{
	//回転させる処理
	rotation.y = PlayerCamera::Get()->mHorizontalRad;
}