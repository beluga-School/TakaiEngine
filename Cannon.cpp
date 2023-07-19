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
	//ターゲットに何か入ったなら
	if (target != nullptr)
	{
		switch (state)
		{
		case Cannon::CannonState::None:
			break;
		case Cannon::CannonState::One:
			//ターゲットを動かす
			//target->position = TEasing::lerp(startPos, interPos, timer.GetTimeRate());
			target->position = Vector3::Spline(inters, timer.GetTimeRate());

			if (timer.GetEnd())
			{
				timer.Start();
				//state = CannonState::Two;
			
				state = CannonState::None;

				target->SetNoGravity(0);

				//移動させ終わったので、ターゲットの保持を解除
				target = nullptr;
			}

			break;
		case Cannon::CannonState::Two:
			//ターゲットを動かす
			target->position = TEasing::lerp(interPos, endPos, timer.GetTimeRate());
		
			if (timer.GetEnd())
			{
				timer.Start();
				state = CannonState::None;
				
				target->SetNoGravity(0);

				//移動させ終わったので、ターゲットの保持を解除
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
	//すでに何か入っているならスキップ
	if (target != nullptr)return;
	
	target = &mob;
	timer.Start();
	state = CannonState::One;
	target->SetNoGravity(1);

	inters.clear();
	inters.push_back(startPos);
	inters.push_back(interPos);
	inters.push_back(endPos);
}

void Cannon::SetState(float maxtime)
{
	timer.mMaxTime = maxtime;
}
