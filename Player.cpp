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
	SetModel(ModelManager::GetModel("beetle"));
}

void Player::Update()
{
	//�ړ��n��������
	moveValue = {0,0,0};

	mCenterVec = matWorld.ExtractAxisZ();
	mSideVec = matWorld.ExtractAxisX();

	//�ړ��ʂ��擾�A���Z
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

	//�{���Z
	position.y = preMove.y;
	position += moveValue;

	//��]�X�V
	RotaUpdate();

	//�X�V
	Obj3d::Update(*Camera::sCamera);
}

void Player::Draw()
{
	Obj3d::DrawMaterial();
}

void Player::Reset()
{
	gravity = 0;
}

void Player::JumpUpdate()
{
	jumpManageTimer.Update();
	stayManageTimer.Update();

	switch (jumpState)
	{
	case Player::JumpState::None:
		//�W�����v���Ă��Ȃ���Ԃœ��͂���������W�����v
		if (Input::Keyboard::TriggerKey(DIK_SPACE) ||
			Pad::TriggerPadButton(PadButton::A))
		{
			//�l���w��
			upJumpS = position.y;
			upJumpE = position.y + jumpPower;

			jumpManageTimer.Start();
			jumpState = JumpState::Up;

			//�d�͂𖳌���
			gravity = 0;
		}

		//���̈ʒu�Ɖ��~����ʒu���قȂ�Ȃ痎��
		if (position.y > downJumpE)
		{
			gravity += gravityAdd;
			preMove.y -= gravity;
		}
		else
		{
			gravity = 0;
			preMove.y = downJumpE;
		}

		break;
	case Player::JumpState::Up:
		//�C�[�W���O�ŏ㏸
		preMove.y = TEasing::OutQuad(upJumpS, upJumpE, jumpManageTimer.GetTimeRate());

		//���Ԃ��I�������X�e�[�g�����̏�ԂɑJ��
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
	//�����蔻��
	Cube pCol;
	pCol.position = position;
	pCol.scale = scale;

	pCol.position.y = preMove.y;

	pCol.position += moveValue;

	for (auto& bCol : Stage::Get()->mColCubes)
	{
		//Cube bCol;
		//bCol.position = bColTemp.position;
		//bCol.scale = bColTemp.scale;

		//�Ȃ���������2�{�Ōv�Z����Ă�̂ō�������������
		//bCol.scale.y /= 2;

		//���̃I�u�W�F�N�g���
		//��ɂ��邩
		bool up = CheckDirections(pCol, bCol, CheckDirection::CD_UP);
		//���ɂ��邩
		bool down = CheckDirections(pCol, bCol, CheckDirection::CD_DOWN);
		//�O�ɂ��邩
		bool center = CheckDirections(pCol, bCol, CheckDirection::CD_CENTER);
		//���ɂ��邩
		bool back = CheckDirections(pCol, bCol, CheckDirection::CD_BACK);
		//���ɂ��邩
		bool left = CheckDirections(pCol, bCol, CheckDirection::CD_LEFT);
		//�E�ɂ��邩
		bool right = CheckDirections(pCol, bCol, CheckDirection::CD_RIGHT);

		//��ʂ̓����蔻��
		if (up)
		{
			//������Ō����Ƃ��ɉ��ɂ���I�u�W�F�N�g�������
			Cube rayCube;
			rayCube.position = pCol.position;
			rayCube.scale = pCol.scale;
			//�X�P�[�����߂�����������΂�
			rayCube.scale.y = 100;

			//���������Ȃ�
			bool cubeCol = Collsions::CubeCollision(rayCube, bCol);

			if (cubeCol)
			{
				//���X�g�ɓ����
				
				//���̃��X�g���������������̂ł͂Ȃ��A
				//�����蔻����Ƃ�A�����v�f�������Ă��Ȃ��Ȃ�����
				//�����蔻�肪�O�ꂽ�Ƃ��ɁA���̗v�f������
				UniqueObjectPushBack(hitList,bCol);
			}
			else
			{
				UniqueObjectErase(hitList,bCol);
			}
		}

		//���ʂ̓����蔻��
		if (down)
		{
			
		}

		//���E�̓����蔻��
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

	//downJumpE = preY;

	

	for (auto& hit : hitList)
	{
		maxY = hit.position.y;
		//�����l�łȂ��A�O�̒l��荂���ʒu�ɂ���Ȃ�
		if (maxY >= preY)
		{
			//�I�_�ʒu���X�V
			downJumpE = hit.position.y + hit.scale.y / 2 + 0.01f;
		}
		preY = hit.position.y;
	}

	checkGUI.Begin({ 500,100 }, { 400,500 });
	ImGui::Text("position x:%f y:%f z:%f", position.x, position.y, position.z);
	ImGui::Text("downJumpE %f", downJumpE);
	for (auto& hit : hitList)
	{
		ImGui::Text("hitlist.position %f %f %f", hit.position.x, hit.position.y, hit.position.z);
		ImGui::Text("hitlist.scale %f %f %f", hit.scale.x, hit.scale.y, hit.scale.z);
	}
	checkGUI.End();

	//�g���I������̂ŏ�����
	//hitList.clear();

	for (auto& bColevent : Stage::Get()->mEventObjects)
	{
		Cube eCol;
		eCol.position = bColevent->position;
		
		//�Ȃ񂩔��肪�����������̂�2�{�� ��������҂����肾�����̂ŁA�ǂ����Ŕ�����
		//���鏈�������܂��Ă�
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
	//��]�����鏈��
	rotation.y = PlayerCamera::Get()->mHorizontalRad;
}