#include "Cannon.h"
#include "ViewProjection.h"
#include "ObjParticle.h"
#include "TimeManager.h"

void Cannon::Initialize()
{
	SetModel(ModelManager::GetModel("Cannon"));
	SetTexture(TextureManager::GetTexture("white"));

	endTargetCircle.Initialize();
	endTargetCircle.SetModel(ModelManager::GetModel("targetMark"));

	endTargetCircle.scale = { 2,2,2 };
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

				for (int i = 0; i < 10; i++)
				{
					ParticleManager::GetInstance()->CreateCubeParticle(target->position, { 5,5,5 }, 10, { 0.1f,0.1f,0.1f,1 });
				}
			}
			break;
		}		
	}

	Obj3d::Update(*Camera::sCamera);

	//常に回しとく
	endTargetCircle.rotation.y += 2.0f * TimeManager::deltaTime;
	endTargetCircle.Update(*Camera::sCamera);
}

void Cannon::Draw()
{
	Obj3d::DrawMaterial();

	if (state != CannonState::None)
	{
		endTargetCircle.DrawMaterial();
	}
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

	endTargetCircle.position = endPos;

	for (int i = 0; i < 10; i++)
	{
		ParticleManager::GetInstance()->CreateCubeParticle(target->position, { 5,5,5 }, 10, { 0.1f,0.1f,0.1f,1 });
	}
}

void Cannon::SetState(float maxtime)
{
	timer.mMaxTime = maxtime;
}
