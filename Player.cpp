#include "Player.h"
#include "Input.h"
#include "TimeManager.h"
#include "Stage.h"
#include "ImguiManager.h"
#include "MathF.h"
#include "PlayerCamera.h"
#include "EnemyManager.h"
#include "ClearDrawScreen.h"
#include "ObjParticle.h"

using namespace Input;

void Player::Initialize()
{
	Obj3d::Initialize();
	SetModel(ModelManager::GetModel("beetle"));

	colDrawer.Initialize();
	colDrawer.SetModel(ModelManager::GetModel("ICOSphere"));
	colDrawer.SetTexture(TextureManager::GetTexture("white"));

	SetOutLineState({ 0.1f,0.1f,0.1f,1.0f }, 0.05f);

	hpGauge.Initialize();
}

void Player::Update()
{
	//�d�͂𖳌���(�f�o�b�O�p)
	//SetNoGravity(true);

	//�_���[�W����
	if (Input::Keyboard::TriggerKey(DIK_T))
	{
		mutekiTimer.Reset();
		DamageEffect(1);
	}
	if (Input::Keyboard::TriggerKey(DIK_G))
	{
		hp.mCurrent += 1;
	}

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

	//Mob���̍X�V
	Mob::CollsionUpdate();

	//Player���L�̓����蔻��X�V(CollideManager�Ɉڂ�)
	//�����u��������܂ł͍������
	ColUpdate();

	//�{���Z
	//position.y = preMove.y;
	position += moveValue;

	//�����蔻��
	///--�G�����蔻��
	mEncountCol.center = position;
	mEncountCol.radius = (scale.x + scale.y + scale.z) / 3.f;

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

	//�����蔻���A�X�e�[�^�X�̍X�V
	DamageUpdate();

	hpGauge.Update();
}

void Player::Draw()
{
	BasicObjectPreDraw(PipelineManager::GetPipeLine("OutLine"), false);
	Obj3d::DrawOutLine();
	
	/*BasicObjectPreDraw(PipelineManager::GetPipeLine("WireFrame"));
	colDrawer.Draw();*/

	BasicObjectPreDraw(PipelineManager::GetPipeLine("Toon"));
	Obj3d::DrawMaterial();
}

void Player::DrawUI()
{
	hpGauge.Draw();
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

void Player::ColUpdate()
{
	///--�n�ʓ����蔻��
	Cube pCol;
	pCol.position = position;
	pCol.scale = scale;

	//pCol.position.y = preMove.y;

	pCol.position += moveValue;

	box.CreateCol(pCol.position, pCol.scale);

	//������ӂ̏������A�S��CollideManager�Ɉڂ�
	//���͂����ɒu���Ȃ��Ɖ��̔��肪���Ȃ��d�g�݂ɂȂ��Ă�̂ŁA���X�������V�X�e��������
	for (auto& col : CollideManager::Get()->allCols)
	{
		if (col->CheckTag(TagTable::Block))
		{
			Block* block = static_cast<Block*>(col);
			CollideManager::Get()->CheckCollide(this, block);
		}
	}

	////�����̓��[�v�u���b�N�̓����蔻��ɂȂ��Ă�
	//for (auto& bColevent : Stage::Get()->mEventObjects)
	//{
	//	Cube eCol;
	//	eCol.position = bColevent->position;
	//	
	//	//�Ȃ񂩔��肪�����������̂�2�{�� ��������҂����肾�����̂ŁA�ǂ����Ŕ����ɂ��鏈�������܂��Ă�
	//	//->����EventBlock�̓����蔻��Ɏg���Ă�X�P�[�����A���f���̕����g���Ă�̂���������Ȃ���
	//	//box���玝���Ă���悤�ɂ���
	//	eCol.scale = bColevent->scale * 2;
	//	
	//	if (Collsions::CubeCollision(eCol, pCol))
	//	{
	//		bColevent->HitEffect();

	//		break;
	//	}
	//}

	//������艺��́AEntity���|�C���^�ŕێ�����悤�ɂ��Ă���C������
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
}

void Player::RotaUpdate()
{
	//���擾���Ȃ�ړ��ł��Ȃ�����
	if (starState != StarState::None)return;

	//��]�����鏈��
	rotation.y = PlayerCamera::Get()->mHorizontalRad;
}

void Player::DamageUpdate()
{
	mutekiTimer.Update();
	flashTimer.Update();

	//���G���Ԓ��͓_�ł�����
	if (mutekiTimer.GetRun())
	{
		if (flashTimer.GetRun() == false)
		{
			flashTimer.Start();
			mIsVisiable = !mIsVisiable;
		}
	}
	//���G���Ԃ��I��������_�ŉ���
	if (mutekiTimer.GetEnd())
	{
		if (mIsVisiable == false)
		{
			mIsVisiable = true;
		}
		mutekiTimer.Reset();
	}

	//�O����HP�̍ő�l�𒴂��鏈���������Ă�����ۂ߂�
	hp.mCurrent = Util::Clamp(hp.mCurrent, -1, MAX_HP);

	//�_���[�W���󂯂��Ƃ�
	if (hp.DecreaseTrigger())
	{
		//�G�t�F�N�g�o��
		for (int i = 0; i < 3; i++)
		{
			ParticleManager::GetInstance()->CreateCubeParticle(position,
				{ 3,3,3 }, 10, { 1,0,0,1 });
		}
	}
	//0�ɂȂ�������ւ���
	if (hp.mCurrent <= 0)
	{
		hp.mCurrent = MAX_HP;
		hpGauge.SetGaugeSize(hp.mCurrent, true);
		Stage::Get()->ChangeLevel(*LevelLoader::Get()->GetData("stage_graveyard"));
	}

	//hp�Q�[�W�̐F��ς��鏈��
	int32_t changeIndex = 0;
	
	if (hp.DecreaseTrigger())
	{
		changeIndex = (MAX_HP - hp.mCurrent) - 1;
		changeIndex = Util::Clamp(changeIndex, 0, MAX_HP);
		
		hpGauge.Addition(-1);
	}
	if (hp.IncreaseTrigger())
	{
		changeIndex = (MAX_HP - hp.mCurrent);
		changeIndex = Util::Clamp(changeIndex, 0, MAX_HP);

		hpGauge.Addition(1);
	}
}

void Player::DamageEffect(int32_t damage)
{
	//���G���Ԓ��Ȃ�X�L�b�v
	if (mutekiTimer.GetRun())return;

	//���G���ԊJ�n
	mutekiTimer.Start();
	//�_���[�W�󂯂�
	hp.mCurrent -= damage;
}

void Player::HPOverFlow(int32_t value)
{
	hp.mCurrent = value;
	//HP�̃I�[�o�[�t���[����
	if (hp.mCurrent > MAX_HP)
	{
		MAX_HP = hp.mCurrent;
		hpGauge.SetGaugeSize(hp.mCurrent, true);
	}
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

int32_t Player::GetNowHP()
{
	return hp.mCurrent;
}

