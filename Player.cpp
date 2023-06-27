#include "Player.h"
#include "Input.h"
#include "TimeManager.h"
#include "Stage.h"
#include "ImguiManager.h"

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

GUI checkHitGUI("checkHit");

void Player::Update()
{
	//移動地を初期化
	moveValue = { 0,0,0 };

	mCenterVec = matWorld.ExtractAxisZ();
	mSideVec = matWorld.ExtractAxisX();

	//重力追加
	moveValue.y -= gravity;

	//移動量を取得、加算
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

	if (Input::Keyboard::TriggerKey(DIK_SPACE))
	{
		moveValue += matWorld.ExtractAxisY() * mSpeed;
	}

	//当たり判定
	Cube pCol;
	pCol.position = position;
	pCol.scale = scale;
	
	pCol.position += moveValue;

	checkHitGUI.Begin({ 100,100 }, { 100,100 });

	for (auto& bCol : Stage::Get()->mObj3ds)
	{
		Cube bColTemp;
		bColTemp.position = bCol.position;
		bColTemp.scale = bCol.scale;
	
		bool up = CheckDirections(pCol, bColTemp, CheckDirection::CD_UP);
		bool down = CheckDirections(pCol, bColTemp, CheckDirection::CD_DOWN);

		bool center = CheckDirections(pCol, bColTemp, CheckDirection::CD_CENTER);
		bool back = CheckDirections(pCol, bColTemp, CheckDirection::CD_BACK);
		bool left = CheckDirections(pCol, bColTemp, CheckDirection::CD_LEFT);
		bool right = CheckDirections(pCol, bColTemp, CheckDirection::CD_RIGHT);

		//上面の当たり判定
		if (up)
		{
			while (Collsions::CubeCollision(pCol, bColTemp))
			{
				pCol.position.y += 0.1f;
				moveValue.y += 0.1f;
				//jumpPower = 0;
				//onGround = true;
			}
		}
		//下面の当たり判定
		if (down)
		{
			while (Collsions::CubeCollision(pCol, bColTemp))
			{
				pCol.position.y -= 0.1f;
				moveValue.y -= 0.1f;
				//jumpPower = 0;
			}
		}

		//左右の当たり判定
		if (up == false)
		{
			if (Collsions::CubeCollision(pCol, bColTemp))
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

		ImGui::Text("up %d", up);
		ImGui::Text("down %d", down);
		ImGui::Text("right %d", right);
		ImGui::Text("left %d", left);
		ImGui::Text("center %d", center);
		ImGui::Text("back %d", back);
	}


	checkHitGUI.End();

	//本加算
	position += moveValue;

	//更新
	Obj3d::Update(*Camera::sCamera);
}

void Player::Draw()
{
	Obj3d::DrawMaterial();
}
