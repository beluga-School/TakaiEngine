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
		//�W�����v����
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
				///---�����ڏ���
				//�ǂ�����������֌�����ς���
				pVec = Player::Get()->position - position;
				pVec.normalize();
				pVec.y = 0;

				rotation.y = standardRotaVec.Radian(pVec);
				if (pVec.z > 0) {
					//�����������𔽑΂�
					rotation.y *= -1;
				}
			}

			//���̈ʒu����
			attackPosS = position;
			//�v���C���[�̈ʒu + ���̏�����(�ړ����� * �ړ�������������) = �I�_���o��
			attackPosE = Player::Get()->position + pVec * attackDistance;

			//�ŏI�ʒu�Ƀ^�[�Q�b�g�}�[�N���o��
			targetMark.position = attackPosE;
			targetMark.position.y = Player::Get()->GetFeet();

			//���������ɖ߂�
			mSpeed = 5.0f;
		}

		break;
	case GEnemy::State::Attacking:
		
		//{
		//	///---�����ڏ���
		//	//�ǂ�����������֌�����ς���
		//	pVec = Player::Get()->position - position;
		//	pVec.normalize();
		//	pVec.y = 0;
	
		//	rotation.y = standardRotaVec.Radian(pVec);
		//	if (pVec.z > 0) {
		//		//�����������𔽑΂�
		//		rotation.y *= -1;
		//	}
		//}

		//���h�ꂷ��
		if (metronomeTimer.GetEnd())
		{
			metronomeTimer.ReverseStart();
		}
		if (metronomeTimer.GetReverseEnd())
		{
			metronomeTimer.Start();
		}
		rotation.x = TEasing::lerp(-MathF::PIf / 4, MathF::PIf / 4, metronomeTimer.GetTimeRate());

		//�}�[�N����]
		targetMark.rotation.y = TEasing::lerp(0, MathF::PIf * 2, markRotaTimer.GetTimeRate());

		///---�ړ�����
		//�i�X����������
		mSpeed += acceleration;
		mSpeed = Util::Clamp(mSpeed, 0.0f, MAX_ACCELERATION);

		position += pVec * mSpeed * TimeManager::deltaTime;

		//position.x = TEasing::InQuad(attackPosS.x,attackPosE.x,attakingTimer.GetTimeRate());
		//position.z = TEasing::InQuad(attackPosS.z,attackPosE.z,attakingTimer.GetTimeRate());

		///---�J�ڏ���
		//�ːi���I�������I���
		//�ڕW�̒n�_�ɓ��B���邩�A�ǂɂԂ�������I���
		//��������ڕW�n�_�����ō��
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
	//�X�e�[�g��None�Ȃ�G���J�E���g�Ɉȍ~
	if (state != State::None)return;
	state = State::Encount;
	encountJumpTimer.Start();
	encountJumpS = position.y;
	encountJumpE = position.y + 2.0f;
}

void GEnemy::ColUpdate()
{
	sphereCol.center = position;
	//�ǐՔ͈͂̋��̔��a
	sphereCol.radius = 8;

	hitSphere.position = sphereCol.center;
	hitSphere.scale = { sphereCol.radius,sphereCol.radius,sphereCol.radius };
}
