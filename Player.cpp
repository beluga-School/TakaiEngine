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

void Player::Initialize()
{
	Obj3d::Initialize();
	SetModel(ModelManager::GetModel("beetle"));

	colDrawer.Initialize();
	colDrawer.SetModel(ModelManager::GetModel("ICOSphere"));
	colDrawer.SetTexture(TextureManager::GetTexture("white"));

	SetOutLineState({ 0.1f,0.1f,0.1f,1.0f }, 0.05f);
}

void Player::Update()
{
	if (PlayerCamera::Get()->GetCamMode() == PlayerCamera::CamMode::Normal)
	{
		starState = StarState::None;
	}
	if (PlayerCamera::Get()->GetCamMode() == PlayerCamera::CamMode::StarGet)
	{
		starState = StarState::Get;
	}


	//�ړ��n��������
	moveValue = { 0,0,0 };

	//�x�N�g����ۑ�
	mCenterVec = matWorld.ExtractAxisZ();
	mSideVec = matWorld.ExtractAxisX();

	Attack();

	//���ړ��X�V
	if (attackState != AttackState::Attacking)
	{
		SideMoveUpdate();
	}
	
	//�c�ړ��X�V(�d�͗����܂�)

	//�W�����v���Ă��Ȃ���Ԃœ��͂���������W�����v
	if (jumpState == JumpState::None)
	{
		if (Input::Keyboard::TriggerKey(DIK_SPACE) ||
			Pad::TriggerPadButton(PadButton::A))
		{
			Jump();
		}
	}

	JumpUpdate();

	//�����蔻��X�V
	ColUpdate();

	//�{���Z
	//position.y = preMove.y;
	position += moveValue;

	//�����蔻��
	///--�G�����蔻��
	playerCol.center = position;
	playerCol.radius = (scale.x + scale.y + scale.z) / 3.f;

	colDrawer.position = position;
	colDrawer.scale = scale;


	//��]�X�V
	//���_�݂����Ȏ~�܂��Ă�Ƃ��͑O������݌v�ɂ������ȁ`�`
	if (attackState != AttackState::Attacking)
	{
		//�X�^�[�擾���͐��ʂ������邽�߉�]�̍X�V���X�g�b�v(���ɏ����Ă���)
		RotaUpdate();
	}

	//�X�V
	Obj3d::Update(*Camera::sCamera);

	colDrawer.Update(*Camera::sCamera);
}

void Player::Draw()
{
	BasicObjectPreDraw(PipelineManager::GetPipeLine("OutLine"), false);
	Obj3d::DrawOutLine();
	
	BasicObjectPreDraw(PipelineManager::GetPipeLine("WireFrame"));
	colDrawer.Draw();

	BasicObjectPreDraw(PipelineManager::GetPipeLine("Toon"));
	Obj3d::DrawMaterial();

}

void Player::Reset()
{
	gravity = 0;
	hitListY.clear();
}

void Player::Attack()
{
	//�U���X�V
	attackingTimer.Update();
	attackCoolTimer.Update();
	attackingMotionTimer.Update();

	//����
	switch (attackState)
	{
	case Player::AttackState::None:
		//�U�����Ă��Ȃ���Ԃœ��͂���������
		//if (Mouse::Triggered(Click::LEFT))
		if(0)
		{
			//�U����ԂɑJ��
			attackState = AttackState::Attacking;
			attackingTimer.Start();

			//�����Ő��ʃx�N�g����ۑ�
			mRolingVec = mCenterVec;

			//���[�����O�����K�p�����悤�ɍő厞�Ԃ�����
			attackingMotionTimer.mMaxTime = attackingTimer.mMaxTime;
			//attackingMotionTimer.mMaxTime = attackingTimer.mMaxTime;
		}
		break;
	case Player::AttackState::Attacking:

		if (attackingMotionTimer.GetRun() == false)
		{
			attackingMotionTimer.Start();
		}

		//���ʃx�N�g���̕����ɐi�߂�
		moveValue += mRolingVec * mRolingSpeed * TimeManager::deltaTime;
		rotation.x = TEasing::InQuad(-MathF::PIf * 2.f, MathF::PIf * 2.f, attackingMotionTimer.GetTimeRate());

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
}

void Player::SideMoveUpdate()
{
	//���擾���Ȃ�ړ��ł��Ȃ�����
	if (starState != StarState::None)return;

	accelerationTimer.Update();
	decelerationTimer.Update();

	//���͂���������A�^�C�}�[�������Ă����A
	//���͂��Ȃ�������A�^�C�}�[�������Ă���

	//�ړ��ʂ��擾�A���Z
	if ((Pad::GetLStickMove().GetLength() != 0 || 
		Pad::GetRStickMove().GetLength() != 0) ||
		(Input::Keyboard::PushKey(DIK_W) || 
		Input::Keyboard::PushKey(DIK_S) || 
		Input::Keyboard::PushKey(DIK_A) || 
		Input::Keyboard::PushKey(DIK_D)))
	{
		if (accelerationTimer.GetStarted() == false)
		{
			accelerationTimer.Start();
		}
		decelerationTimer.Reset();
	
		//�����𑬓x�ɔ��f
		mSpeed = TEasing::InQuad(0.0f, MAX_SPEED, accelerationTimer.GetTimeRate());
	
		//�O��̈ړ��������L�^(�L�[�{�[�h�̂�)
		oldMoveVec += mCenterVec * static_cast<float>((Input::Keyboard::PushKey(DIK_W) - Input::Keyboard::PushKey(DIK_S)));
		oldMoveVec += mSideVec * static_cast<float>((Input::Keyboard::PushKey(DIK_D) - Input::Keyboard::PushKey(DIK_A)));

		oldMoveVec.normalize();
	}
	else if(oldMoveVec.length() != 0)//�O�t���[���ŃL�[�{�[�h���͂���������(�p�b�h�Ȃ猸���Ȃ�)
	{
		if (decelerationTimer.GetStarted() == false)
		{
			decelerationTimer.Start();
			//���x��ۑ�
			mSaveSpeed = mSpeed;
		}
		accelerationTimer.Reset();

		//�����𑬓x�ɔ��f
		mSpeed = TEasing::InQuad(mSaveSpeed,0.0f, decelerationTimer.GetTimeRate());
	}

	//�������̈ړ�
	if (decelerationTimer.GetRun())
	{
		moveValue += oldMoveVec * mSpeed * TimeManager::deltaTime;
	}

	if (Input::Pad::CheckConnectPad())
	{
		moveValue += mCenterVec * Input::Pad::GetLStickMove().y * mSpeed * TimeManager::deltaTime;
		moveValue += mSideVec * Input::Pad::GetLStickMove().x * mSpeed * TimeManager::deltaTime;
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

	//���Ƃňړ����x�̉��Z�ɏ���t����
}

//void Player::JumpUpdate()
//{
//	jumpManageTimer.Update();
//	stayManageTimer.Update();
//
//	switch (jumpState)
//	{
//	case Player::JumpState::None:
//		
//		//�W�����v���Ă��Ȃ���Ԃœ��͂���������W�����v
//		if (Input::Keyboard::TriggerKey(DIK_SPACE) ||
//			Pad::TriggerPadButton(PadButton::A))
//		{
//			Jump();
//		}
//		
//		if (position.y > hitCubeMaxY)
//		{
//			jumpState = JumpState::Down;
//		}
//		else
//		{
//			//�n�ʂɗ����Ă����Ԃɂ���
//			gravity = 0;
//		}
//
//		break;
//	case Player::JumpState::Up:
//		//�C�[�W���O�ŏ㏸
//		position.y = TEasing::OutQuad(upJumpS, upJumpE, jumpManageTimer.GetTimeRate());
//
//		//���Ԃ��I�������X�e�[�g�����̏�ԂɑJ��
//		if (jumpManageTimer.GetEnd())
//		{
//			jumpState = JumpState::Staying;
//			jumpManageTimer.Reset();
//			stayManageTimer.Start();
//		}
//
//		break;
//	case Player::JumpState::Staying:
//		if (stayManageTimer.GetEnd())
//		{
//			jumpState = JumpState::None;
//			stayManageTimer.Reset();
//		}
//
//		break;
//	case Player::JumpState::Down:
//		//hitList�̒��ŁA�ł������ʒu�ɂ���I�u�W�F�N�g��莩�g�̍��W������������
//		if (position.y > hitCubeMaxY)
//		{
//			//�d�͗���������
//			gravity += gravityAdd;
//			position.y -= gravity * TimeManager::deltaTime;
//		}
//		//hitList�I�u�W�F�N�g�̒��ŁA�ł������ʒu�ɂ���I�u�W�F�N�g�Ɏ��g���������Ă���Ȃ�
//		else
//		{
//			jumpState = JumpState::None;
//		}
//		
//		break;
//	}
//}

void Player::ColUpdate()
{
	///--�n�ʓ����蔻��
	Cube pCol;
	pCol.position = position;
	pCol.scale = scale;

	//pCol.position.y = preMove.y;

	pCol.position += moveValue;

	//for (auto& bColTemp : Stage::Get()->mColCubes)

	CreateCol(pCol.position, pCol.scale);
	for (auto& bColTemp : Stage::Get()->mColObj3ds)
	{
		Block* block = static_cast<Block*>(&bColTemp);
		CollideManager::Get()->CheckCollide(this, block);
	}

	GroundCol();

	////������̔���
	//float preY = -114514.f;
	//float maxY = 0;

	//hitCubeMaxY = preY;

	//for (auto& hit : hitListY)
	//{
	//	maxY = hit.position.y;
	//	//�����l�łȂ��A�O�̒l��荂���ʒu�ɂ���Ȃ�
	//	if (maxY >= preY)
	//	{
	//		//��ԍ������W���Z�o
	//		//���������������āA�u���b�N�̐؂�ڂɈ���������Ȃ��悤��
	//		feet = hit.position.y + hit.scale.y / 2;
	//		hitCubeMaxY = feet + pCol.scale.y / 2 + 0.01f;
	//	}
	//	preY = hit.position.y;
	//}

	////�������Z�o�����ő�l�ɍ��킹��
	//if (jumpState == JumpState::None)
	//{
	//	if (position.y > hitCubeMaxY)
	//	{
	//		jumpState = JumpState::Down;
	//	}
	//	else
	//	{
	//		position.y = hitCubeMaxY;
	//	}
	//}

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

	for (auto& star : StarManager::Get()->mStars)
	{
		Cube eCol;
		eCol.position = star->position;

		//�Ȃ񂩔��肪�����������̂�2�{�� ��������҂����肾�����̂ŁA�ǂ����Ŕ�����
		//���鏈�������܂��Ă�
		eCol.scale = star->scale * 2;

		if (Collsions::CubeCollision(eCol, pCol))
		{
			star->HitEffect();

			break;
		}
	}

	for (auto& goal : Stage::Get()->mGoals)
	{
		Cube goalCol;
		goalCol.position = goal->position + goal->goalBlock.position;

		//�Ȃ񂩔��肪�����������̂�2�{�� ��������҂����肾�����̂ŁA�ǂ����Ŕ�����
		//���鏈�������܂��Ă�
		goalCol.scale = goal->goalBlock.scale * 2;

		if (Collsions::CubeCollision(goalCol, pCol))
		{
			goal->HitEffect();

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
		}
		
		if (Collsions::SphereCollsion(playerCol, enemy->sphereCol))
		{
			enemy->Encount();
		}
	}
}

void Player::RotaUpdate()
{
	//���擾���Ȃ�ړ��ł��Ȃ�����
	if (starState != StarState::None)return;

	//��]�����鏈��
	rotation.y = PlayerCamera::Get()->mHorizontalRad;
}

void Player::Jump()
{
	//�l���w��
	upJumpS = position.y;
	upJumpE = position.y + jumpPower;

	jumpManageTimer.Start();
	jumpState = JumpState::Up;

	//�d�͂𖳌���
	gravity = 0;
}