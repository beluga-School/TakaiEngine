#include "Cannon.h"
#include "ViewProjection.h"

void Cannon::Initialize()
{
	SetModel(ModelManager::GetModel("Cannon"));
	SetTexture(TextureManager::GetTexture("white"));
}

void Cannon::Update()
{
	CreateCol();
	timer.Update();
	//�^�[�Q�b�g�ɉ����������Ȃ�
	if (target != nullptr)
	{
		switch (state)
		{
		case Cannon::CannonState::None:
			break;
		case Cannon::CannonState::One:
			//�^�[�Q�b�g�𓮂���
			target->position = TEasing::InQuad(startPos, interPos, timer.GetTimeRate());
			

			if (timer.GetEnd())
			{
				timer.Start();
				state = CannonState::Two;
			}

			break;
		case Cannon::CannonState::Two:
			//�^�[�Q�b�g�𓮂���
			target->position = TEasing::InQuad(interPos, endPos, timer.GetTimeRate());
		
			if (timer.GetEnd())
			{
				timer.Start();
				state = CannonState::None;
				
				target->SetNoGravity(0);

				//�ړ������I������̂ŁA�^�[�Q�b�g�̕ێ�������
				target = nullptr;

			}
			break;
		}		
	}

	Obj3d::Update(*Camera::sCamera);
}

void Cannon::Draw()
{
	Obj3d::DrawMaterial();
}

void Cannon::OnCollide(Mob& mob)
{
	//���łɉ��������Ă���Ȃ�X�L�b�v
	if (target != nullptr)return;
	
	target = &mob;
	timer.Start();
	state = CannonState::One;
	target->SetNoGravity(1);
}

void Cannon::SetState(float maxtime)
{
	timer.mMaxTime = maxtime;
}
