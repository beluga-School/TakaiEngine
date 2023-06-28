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
	//�ړ��n��������
	moveValue = {0,0,0};

	mCenterVec = matWorld.ExtractAxisZ();
	mSideVec = matWorld.ExtractAxisX();

	//�ړ��ʂ��擾�A���Z
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

	jumpManageTimer.Update();
	stayManageTimer.Update();
	
	switch (jumpState)
	{
	case Player::JumpState::None:
		//�W�����v���Ă��Ȃ���Ԃœ��͂���������W�����v
		if (Input::Keyboard::TriggerKey(DIK_SPACE))
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
		if (downJumpE < position.y)
		{
			jumpState = JumpState::Down;
			jumpManageTimer.Start();
			stayManageTimer.Reset();

			//���݈ʒu���n�_��
			downJumpS = position.y;
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
			jumpState = JumpState::Down;
			jumpManageTimer.Start();
			stayManageTimer.Reset();

			//���݈ʒu���n�_��
			downJumpS = position.y;
		}

		break;
	case Player::JumpState::Down:
		//�C�[�W���O�ŏ㏸
		preMove.y = TEasing::InQuad(downJumpS, downJumpE, jumpManageTimer.GetTimeRate());

		//���Ԃ��I�������X�e�[�g�����̏�ԂɑJ��
		if (jumpManageTimer.GetEnd())
		{
			jumpState = JumpState::End;
			jumpManageTimer.Reset();
		}

		break;
	case Player::JumpState::End:
		//�������Ă��Ȃ���Ԃɖ߂�(�N�[���^�C���Ƃ��t����Ȃ炱���ŕt����)
		jumpState = JumpState::None;

		break;
	}
	//else
	//{
	//	//�d�͂����Z
	//	gravity += gravityAdd;
	//	
	//	//�d�͂�K�p
	//	preMove.y -= gravity;
	//}

	//�f�o�b�O�d�͉��Z
	if (Input::Keyboard::PushKey(DIK_LSHIFT))
	{
		preMove.y -= gravityAdd;
	}
	if (Input::Keyboard::PushKey(DIK_LCONTROL))
	{
		preMove.y += gravityAdd;
	}

	//�����蔻��
	Cube pCol;
	pCol.position = position;
	pCol.scale = scale;
	
	pCol.position.y = preMove.y;

	pCol.position += moveValue;

	for (auto& bColTemp : Stage::Get()->mObj3ds)
	{	
		Cube bCol;
		bCol.position = bColTemp.position;
		bCol.scale = bColTemp.scale;

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
			rayCube.scale.y = 1000;

			//���������Ȃ�
			if (Collsions::CubeCollision(rayCube, bCol))
			{
				//���X�g�ɓ����
				hitList.push_back(bCol);
			}

			/*if(Collsions::CubeCollision(pCol, bColTemp))
			{
				preMove.y = bColTemp.position.y;
			}*/
		}

		////���ʂ̓����蔻��
		//if (down)
		//{
		//	while (Collsions::CubeCollision(pCol, bColTemp))
		//	{
		//		pCol.position.y -= 0.1f;
		//		moveValue.y -= 0.1f;
		//	}
		//}

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
	downJumpE = -3.f;
	for (auto &hit : hitList)
	{
		maxY = hit.position.y;
		//�����l�łȂ��A�O�̒l��荂���ʒu�ɂ���Ȃ�
		if (maxY >= preY)
		{
			//�I�_�ʒu���X�V
			//TODO:�I�_�ʒu�����~���ɂ��X�V�����̂ŁA�Ȃ񂩃o�O�肻��
			downJumpE = hit.position.y + hit.scale.y;
		}
		preY = hit.position.y;
	}

	//�g���I������̂ŏ�����
	hitList.clear();

	checkHitGUI.Begin({ 100,100 }, { 100,100 });
	ImGui::Text("downJumpE %f", downJumpE);
	ImGui::Text("state %d", jumpState);
	checkHitGUI.End();

	//�{���Z
	position.y = preMove.y;
	position += moveValue;

	//�X�V
	Obj3d::Update(*Camera::sCamera);
}

void Player::Draw()
{
	Obj3d::DrawMaterial();
}
