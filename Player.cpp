#include "Player.h"
#include "Input.h"
#include "TimeManager.h"
#include "Stage.h"
#include "ImguiManager.h"
#include "MathF.h"
#include "PlayerCamera.h"
#include "EnemyManager.h"

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
	SetModel(ModelManager::GetModel("firewisp"));

	position.y = 10.0f;
}

void Player::Update()
{
	//移動地を初期化
	moveValue = {0,0,0};

	mCenterVec = matWorld.ExtractAxisZ();
	mSideVec = matWorld.ExtractAxisX();

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
	
	JumpUpdate();

	ColUpdate();

	//本加算
	position.y = preMove.y;
	position += moveValue;

	//回転更新
	RotaUpdate();

	//更新
	Obj3d::Update(*Camera::sCamera);
}

void Player::Draw()
{
	Obj3d::DrawMaterial();
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

		//今の位置と下降判定位置が異なるなら落下
		if (downJumpE < position.y)
		{
			jumpState = JumpState::Down;
			jumpManageTimer.Start();
			stayManageTimer.Reset();

			//現在位置を始点に
			downJumpS = position.y;
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
			jumpState = JumpState::Down;
			jumpManageTimer.Start();
			stayManageTimer.Reset();

			//現在位置を始点に
			downJumpS = position.y;
		}

		break;
	case Player::JumpState::Down:
		//イージングで上昇
		preMove.y = TEasing::InQuad(downJumpS, downJumpE, jumpManageTimer.GetTimeRate());

		//時間が終わったらステートを次の状態に遷移
		if (jumpManageTimer.GetEnd())
		{
			jumpState = JumpState::End;
			jumpManageTimer.Reset();
		}

		break;
	case Player::JumpState::End:
		//何もしていない状態に戻す(クールタイムとか付けるならここで付ける)
		jumpState = JumpState::None;

		break;
	}
}

void Player::ColUpdate()
{
	//当たり判定
	Cube pCol;
	pCol.position = position;
	pCol.scale = scale;

	pCol.position.y = preMove.y;

	pCol.position += moveValue;

	for (auto& bColTemp : Stage::Get()->mColObj3ds)
	{
		Cube bCol;
		bCol.position = bColTemp.position;
		bCol.scale = bColTemp.scale;

		//なぜか高さが2倍で計算されてるので高さだけ半分に
		bCol.scale.y /= 2;

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
			rayCube.scale.y = 1000;

			//当たったなら
			if (Collsions::CubeCollision(rayCube, bCol))
			{
				//リストに入れる
				hitList.push_back(bCol);
			}

			/*if(Collsions::CubeCollision(pCol, bColTemp))
			{
				preMove.y = bColTemp.position.y;
			}*/
		}

		////下面の当たり判定
		//if (down)
		//{
		//	while (Collsions::CubeCollision(pCol, bColTemp))
		//	{
		//		pCol.position.y -= 0.1f;
		//		moveValue.y -= 0.1f;
		//	}
		//}

		//左右の当たり判定
		if (up == false)
		{
			if (Collsions::CubeCollision(pCol, bCol))
			{
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
		}
	}

	float preY = -114514.f;
	float maxY = 0;
	downJumpE = -3.f;
	for (auto& hit : hitList)
	{
		maxY = hit.position.y;
		//初期値でなく、前の値より高い位置にあるなら
		if (maxY >= preY)
		{
			//終点位置を更新
			//TODO:終点位置が下降中にも更新されるので、なんかバグりそう
			downJumpE = hit.position.y + hit.scale.y;
		}
		preY = hit.position.y;
	}

	//使い終わったので初期化
	hitList.clear();

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
