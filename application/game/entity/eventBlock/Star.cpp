#include "Star.h"
#include "PlayerCamera.h"
#include "Player.h"
#include "TimeManager.h"
#include "MathF.h"
#include "ObjParticle.h"
#include "GameUIManager.h"
#include "StageChanger.h"

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

	float randScale = 0.0f;

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
				ParticleManager::Get()->CreateCubeParticle(position, { 1.5f,1.5f,1.5f }, 2.0f, { 1.0f,1.0f,0.0f,1 });
			}
		}

		break;
	case Star::StarState::jumpUp:
		//1秒間に2回転する
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

		//1秒間に2回転する
		rotaSpeed = 2;

		scale = TEasing::OutQuad(jumpUpScale, { 0,0,0 }, inholeTimer.GetTimeRate());
		position = TEasing::OutQuad(jumpUpPos, Player::Get()->position, inholeTimer.GetTimeRate());

		if (inholeTimer.GetEnd())
		{
			starState = StarState::CountDown;
			for (int i = 0; i < 10; i++)
			{
				randScale = MathF::GetRand(1.5f,2.0f);
				ParticleManager::Get()->CreateCubeParticle(position, { randScale,randScale,randScale }, 10.0f, { 1.0f,1.0f,0.0f,1 });
			}
		}

		break;
	case Star::StarState::CountDown:
		GameUIManager::Get()->starUI.CountDown();
		starState = StarState::End;
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

	jumpUpTimer.Start();
	delayTimer.Reset();
	inholeTimer.Reset();

	starState = StarState::jumpUp;
}

void Star::StateEnd()
{
	if (starState == StarState::CountDown)
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

void Star::SetCorrected()
{
	starState = StarState::End;
	hit = true;
	scale = { 0,0,0 };
}

void Star::PopStar(const Vector3& pop)
{
	LevelData::ObjectData data;
	data.translation = pop;
	data.scaling = { 1,1,1 };
	data.rotation = { 0,0,0 };
	data.collider.center = { 0,0,0 };
	data.collider.size = { 0.25f,1.5f,1.5f };
	data.fileName = "star";
	data.eventtrigerName = "star";
	StageChanger::Get()->SetObject<Star>(data);

	StageChanger::Get()->CollisionSet(data);

	GameUIManager::Get()->starUI.CountUp();
}

Star* Star::EventPopStar(const std::string& eventname)
{
	for (auto& star : StageChanger::Get()->mEntitys)
	{
		//呼び出しイベント名と持っているイベント名が一致する個体を呼び出す
		if (eventname == star->eventName_)
		{
			if (!star->mActive) {
				Star* temp = static_cast<Star*>(star.get());
				temp->mActive = true;
				return temp;
			}
		}
	}

	return nullptr;
}

void Star::EventVanishStar(const std::string& eventname)
{
	for (auto& star : StageChanger::Get()->mEntitys)
	{
		//呼び出しイベント名と持っているイベント名が一致する個体を呼び出す
		if (eventname == star->eventName_)
		{
			if (star->mActive) {
				star->mActive = false;
			}
		}
	}
}

bool Star::EventCheckStar(const std::string& eventname)
{
	for (auto& star : StageChanger::Get()->mEntitys)
	{
		//呼び出しイベント名と持っているイベント名が一致する個体を呼び出す
		if (eventname == star->eventName_)
		{
			//取得されているならtrue まだ未取得ならfalseを返す
			Star* check = static_cast<Star*>(star.get());
			return check->hit;
		}
	}

	return false;
}

void Star::Vanish()
{
	mActive = false;
}

