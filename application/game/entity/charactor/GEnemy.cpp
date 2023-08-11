#include "GEnemy.h"
#include "Player.h"
#include "TimeManager.h"
#include "ClearDrawScreen.h"
#include "MathF.h"
#include "ObjParticle.h"

void GEnemy::Initialize()
{
	Obj3d::Initialize();
	SetModel(ModelManager::GetModel("firewisp"));
	SetTexture(TextureManager::GetTexture("white"));
	SetOutLineState({0,0,0,1},0.05f);

	EncountSphereInitialize();

	Register();
}

void GEnemy::Update()
{
	stayTimer.Update();
	metronomeTimer.Update();
	encountJumpTimer.Update();
	accelerationTimer.Update();

	deadTimer.Update();

	Vector3 standardRotaVec = {MathF::PIf / 2,0,0};

	switch (mActTable)
	{
		
	case ActTable::None:
		rotation.y = 0;
		break;
	case ActTable::Encount:
		SetNoGravity(true);

		//�W�����v����
		position.y = TEasing::OutQuad(encountJumpS, encountJumpE, encountJumpTimer.GetTimeRate());

		if (encountJumpTimer.GetEnd())
		{
			encountJumpTimer.ReverseStart();
		}
		if (encountJumpTimer.GetReverseEnd())
		{
			mActTable = ActTable::Tracking;
			metronomeTimer.Start();
			accelerationTimer.Start();
			SetNoGravity(false);
		}

		break;
	case ActTable::Tracking:
		
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

		///---�ړ�����
		//�i�X����������
		mSpeed = TEasing::InQuad(0.0f, MAX_ACCELERATION, accelerationTimer.GetTimeRate());

		position += pVec * mSpeed * TimeManager::deltaTime;

		///---�J�ڏ���
		//�U���͈͂���O�ꂽ��I���
		if (!Collsions::SphereCollsion(Player::Get()->mEncountCol, sphereCol))
		{
			mActTable = ActTable::Staying;
			stayTimer.Start();
		}

		break;
	case ActTable::Staying:
		
		if (stayTimer.GetEnd())
		{
			mActTable = ActTable::None;
			stayTimer.Reset();
		}

		break;
	case ActTable::Dead:
		position.x = TEasing::InQuad(deadEasingS.x, deadEasingE.x, deadTimer.GetTimeRate());
		position.y = TEasing::InQuad(deadEasingS.y, deadEasingE.y, deadTimer.GetTimeRate());
		position.z = TEasing::InQuad(deadEasingS.z, deadEasingE.z, deadTimer.GetTimeRate());

		//y��0��
		rotation.x = 0;
		rotation.y = 0;

		//��]�����Z
		rotation.z += deadRoring * TimeManager::deltaTime;

		if (deadTimer.GetEnd())
		{
			isDead = true;
			//�p�[�e�B�N���o������
			//�T�C�Y���傫���Ȃ�ɂ�Ĕ����ʂ�������
			int32_t partnum = static_cast<int32_t>(scale.x + scale.y + scale.z) / 3 * 3;
			float partscale = (scale.x + scale.y + scale.z) / 9;
			float partdistance = (scale.x + scale.y + scale.z) / 3 * 10;
			//�ꉞ������������t����
			partnum = Util::Clamp(partnum, 3, 50);
			for (int32_t i = 0; i < partnum; i++)
			{
				ParticleManager::GetInstance()->CreateCubeParticle(position,
					{ partscale,partscale,partscale }, partdistance, { 0,1,0,0.5f });
			}
		}

		break;
	}
	//���炵���������Z����
	box.CreateCol(position + saveColCenter, box.scale);
	box.ColDrawerUpdate(position + saveColCenter, box.scale);

	CollsionUpdate();

	EncountSphereUpdate();

	Obj3d::Update(*Camera::sCamera);
}

void GEnemy::Draw()
{
	Obj3d::DrawMaterial();
}

void GEnemy::HitEffect()
{
	//���łɎ��S�ς݂Ȃ�X�L�b�v
	if (IsDead())return;
	mActTable = ActTable::Dead;
	
	//���S���̃v���C���[�������Ă���������ۑ�
	deadDirection = Player::Get()->matWorld.ExtractAxisZ();
	
	//�΂ߏ�ɐ������ł����悤��
	float rand = MathF::GetRand(6.0f, 6.5f);

	deadDirection.y = MathF::PIf / rand;

	deadEasingS = position;

	//�X�P�[������艓���ɐ�����Ԃ悤��
	float scaledistance = (scale.x + scale.y + scale.z) / 3 * 0.5f;

	//���ʂɌ������Đ�����΂����悤�ɏI�_��ݒ�
	deadEasingE = position + deadDirection * 7.0f * scaledistance;

	deadTimer.Start();
}

void GEnemy::Encount()
{
	//�X�e�[�g��None�Ȃ�G���J�E���g�Ɉȍ~
	if (mActTable != ActTable::None)return;
	mActTable = ActTable::Encount;
	encountJumpTimer.Start();
	encountJumpS = position.y;
	encountJumpE = position.y + 2.0f;
}