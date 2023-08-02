#include "Star.h"
#include "PlayerCamera.h"
#include "Player.h"
#include "TimeManager.h"
#include "MathF.h"
#include "ObjParticle.h"

void Star::Initialize()
{
	SetModel(ModelManager::GetModel("star"));
	SetTexture(TextureManager::GetTexture("white"));
}

void Star::Update()
{
	particleTimer.Update();
	jumpUpTimer.Update();
	inholeTimer.Update();
	delayTimer.Update();

	//��������ʋ��� starUI�ōs��
	//�@(�@�̎擾�Ɠ����^�C�~���O��)star�̕�����UI���o��
	//�Astar�����ꂫ������Get!�̕�����UI���o��
	//�A.5 �h�肳������Ȃ������炱���ŉ�ʑS�̂����؂�с[�Ƃ�����IUI���o��
	//�B�X�^�[�̉摜�𔒂œh��Ԃ�������o��
	//�C �@�A�B���g�傷�� �g�債���������ɒʏ�̃X�^�[�̉摜���g�傹���ɏo��
	//�D �@�A�B���k������

	float randScale = 0.0f;

	switch (starState)
	{
	case Star::StarState::None:
		//1�b�Ԃ�0.25��]���Ă�
		rotaSpeed = 0.25f;
		if (particleTimer.GetRun() == false)
		{
			particleTimer.Start();
			for (int i = 0; i < 3; i++)
			{
				ParticleManager::GetInstance()->CreateCubeParticle(position, { 1.5f,1.5f,1.5f }, 2.0f, { 1.0f,1.0f,0.0f,1 });
			}
		}

		break;
	case Star::StarState::jumpUp:
		//1�b�Ԃ�2��]����
		rotaSpeed = 2;

		jumpUpPos = saveStartPos;
		jumpUpPos.y += 10.0f;

		jumpUpScale = saveScale * 1.5f;

		scale = TEasing::OutQuad(saveScale, jumpUpScale, jumpUpTimer.GetTimeRate());
		position = TEasing::OutQuad(saveStartPos, jumpUpPos, jumpUpTimer.GetTimeRate());

		
		if (jumpUpTimer.GetEnd())
		{
			if (delayTimer.GetStarted() == false)delayTimer.Start();
		}
		if (delayTimer.GetEnd())
		{
			starState = StarState::Inhole;
			inholeTimer.Start();
		}

		break;
	case Star::StarState::Inhole:

		//1�b�Ԃ�2��]����
		rotaSpeed = 2;

		scale = TEasing::OutQuad(jumpUpScale, { 0,0,0 }, inholeTimer.GetTimeRate());
		position = TEasing::OutQuad(jumpUpPos, Player::Get()->position, inholeTimer.GetTimeRate());

		if (inholeTimer.GetEnd())
		{
			starState = StarState::CountUp;
			for (int i = 0; i < 10; i++)
			{
				randScale = MathF::GetRand(1.5f,2.0f);
				ParticleManager::GetInstance()->CreateCubeParticle(position, { randScale,randScale,randScale }, 10.0f, { 1.0f,1.0f,0.0f,1 });
			}
		}

		break;
	case Star::StarState::CountUp:
		break;
	case Star::StarState::End:
	
		break;
	}

	rotation.y += (MathF::PIf * 2) * rotaSpeed * TimeManager::deltaTime;

	Obj3d::Update(*Camera::sCamera);
}

void Star::Draw()
{
	Obj3d::DrawMaterial();
}

void Star::HitEffect()
{
	//2��ڂ͓���Ȃ��悤��
	if (hit)return;

	saveScale = scale;

	saveStartPos = position;
	savePlayerPos = Player::Get()->position;

	hit = true;

	jumpUpTimer.Start();
	delayTimer.Reset();
	inholeTimer.Reset();

	starState = StarState::jumpUp;
}

void Star::StateEnd()
{
	if (starState == StarState::CountUp)
	{
		starState = StarState::End;
	}
}

bool Star::InHoleEnd()
{
	return inholeTimer.GetEnd();
}

Star::StarState Star::GetState()
{
	return starState;
}
