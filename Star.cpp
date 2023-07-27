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
	timer.Update();

	switch (starState)
	{
	case Star::StarState::None:
		//1秒間に0.25回転してる
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
	case Star::StarState::Inhole:
		if (timer.GetStarted() == false)
		{
			timer.Start();
		}

		//1秒間に2回転する
		rotaSpeed = 2;

		scale = TEasing::OutQuad(saveScale, { 0,0,0 }, timer.GetTimeRate());
		position = TEasing::OutQuad(saveStartPos, Player::Get()->position, timer.GetTimeRate());

		if (timer.GetEnd())
		{
			starState = StarState::End;
			//ステート移行の際に、全体の進行を一つ先に進める
			StarManager::Get()->JumpMove();
		}

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
	//2回目は入らないように
	if (hit)return;

	saveScale = scale;

	saveStartPos = position;
	savePlayerPos = Player::Get()->position;

	hit = true;

	StarManager::Get()->Start();
}

void StarManager::JumpMove()
{
	progress = StarGetState::Jumping;
	jumpingTimer.Start();
	
	Player::Get()->Jump();
	jumpingCount++;
}
void StarManager::Update()
{
	for (auto& obj : mStars)
	{
		obj->Update();
	}

	jumpingTimer.Update();

	switch (progress)
	{
	case StarManager::StarGetState::None:
		break;
	case StarManager::StarGetState::MoveCam:
		PlayerCamera::Get()->ChangeStarGetMode();
		if (PlayerCamera::Get()->CamChangeEnd())
		{
			progress = StarGetState::Inhole;
		}
		break;
	case StarManager::StarGetState::Inhole:
		//当たったオブジェクトを吸い込む
		for (auto &star : mStars)
		{
			if (star->starState == Star::StarState::None)
			{
				if (star->hit)
				{
					star->starState = Star::StarState::Inhole;
				}
			}
		}

		break;
	case StarManager::StarGetState::Jumping:

		if (jumpingTimer.GetEnd())
		{
			jumpingCount++;
			if (jumpingCount > 1)
			{
				progress = StarGetState::BackCam;
			}
			else
			{
				jumpingTimer.Start();
				Player::Get()->Jump();
			}
		}

		break;
	case StarManager::StarGetState::BackCam:

		//演出が終わったらカメラを元に戻す
		//イージングつけてないので付ける
		PlayerCamera::Get()->ChangeNormalMode();
		progress = StarGetState::None;
		break;
	}
}
