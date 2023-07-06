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
	attackTimer.Update();
	stayTimer.Update();
	metronomeTimer.Update();
	encountJumpTimer.Update();
	trackingTimer.Update();

	Vector3 pVec{};
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
			state = State::Traking;
			metronomeTimer.Start();
			trackingTimer.Reset();
		}

		break;
	case GEnemy::State::Traking:
		
		///---�����ڏ���
		//�ǂ�����������֌�����ς���
		pVec = Player::Get()->position - position;
		pVec.normalize();
		pVec.y = 0;	

		position += pVec * mSpeed * TimeManager::deltaTime;
		rotation.y = standardRotaVec.Radian(pVec);
		if (pVec.z > 0) {
			//�����������𔽑΂�
			rotation.y *= -1;
		}

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

		///---�J�ڏ���
		//�O��Ă�Ȃ�ǐՉ����^�C�}�[���J�n
		if (!Collsions::SphereCollsion(Player::Get()->playerCol, sphereCol))
		{
			if (trackingTimer.GetStarted() == false)
			{
				trackingTimer.Start();
			}
		}
		//�^�C�}�[�I�����ɍēx������s���A�O��Ă�Ȃ�ǐՉ���
		if (trackingTimer.GetEnd())
		{
			if (!Collsions::SphereCollsion(Player::Get()->playerCol, sphereCol))
			{
				state = State::None;
			}
			else
			{
				trackingTimer.Reset();
			}
		}

		//�����̍U�����[�h�J�ڂ̓����蔻��ɒB������U���ɑJ��
		if (Collsions::SphereCollsion(Player::Get()->playerCol, attackSphereCol))
		{
			state = State::Attacking;
			attackTimer.Start();
		}

		break;
	case GEnemy::State::Attacking:

		//��яオ���āA�^�C�}�[�̌o�ߎ��Ԃ��I�������U�����肪�ł�
		if (attackTimer.GetEnd())
		{
			state = State::Staying;
			attackTimer.Reset();
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
}

void GEnemy::Draw()
{
	BasicObjectPreDraw(PipelineManager::GetPipeLine("Toon"));
	Obj3d::DrawMaterial();

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
	sphereCol.radius = 8;

	attackSphereCol.center = position;
	attackSphereCol.radius = 3;

	hitSphere.position = sphereCol.center;
	hitSphere.scale = { sphereCol.radius,sphereCol.radius,sphereCol.radius };
}
