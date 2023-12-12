#include "RedCoin.h"
#include "StageChanger.h"
#include "TimeManager.h"
#include "ObjParticle.h"

void RedCoin::Initialize()
{
	SetModel(ModelManager::GetModel("Coin"));
	SetTexture(ModelManager::GetModel("Coin")->mMaterial.mTexture.get());
	color_ = { 1,0,0,1 };

	savePos = position;
}

void RedCoin::Update()
{
	getTimer.Update();

	rotation.y += 3.14f * TimeManager::deltaTime;

	position.y = TEasing::OutQuad(savePos.y, savePos.y + 4.f,getTimer.GetTimeRate());

	if (getTimer.GetNowEnd())
	{
		for (int32_t i = 0; i < 20; i++)
		{
			ParticleManager::Get()->CreateCubeParticle(position, { 2,2,2 }, 10.0f, { 1, 0, 0, 1 });
		}
		scale = { 0,0,0 };
		mActive = false;
	}

	Obj3d::Update(*Camera::sCamera);
}

void RedCoin::Draw()
{
	Obj3d::Draw();
}

void RedCoin::HitEffect()
{
	//2回目は入らないように
	if (hit)return;

	getTimer.Start();

	hit = true;
}

void RedCoin::PopEventRedCoin(const std::string& eventname)
{
	for (auto& coin : StageChanger::Get()->mEntitys)
	{
		//出現させようとしているものが赤コインか確認し、違うなら返す
		if (!coin->CheckTag(TagTable::RedCoin)) {
			continue; 
		}
		//呼び出しイベント名と持っているイベント名が一致する個体を呼び出す
		if (eventname == coin->eventName_)
		{
			if (!coin->mActive) {
				RedCoin* temp = static_cast<RedCoin*>(coin.get());
				temp->mActive = true;
			}
		}
	}
}

bool RedCoin::CheckEventRedCoin(const std::string& eventname)
{
	for (auto& temp : StageChanger::Get()->mEntitys)
	{
		//出現させようとしているものが赤コインか確認
		if (!temp->CheckTag(TagTable::RedCoin)) {
			continue;
		}

		//呼び出しイベント名と持っているイベント名が一致する個体を呼び出す
		if (eventname == temp->eventName_ && temp->mActive)
		{
			return true;
		}
	}

	return false;
}

void RedCoin::DeleteEventRedCoin(const std::string& eventname)
{
	for (auto& temp : StageChanger::Get()->mEntitys)
	{
		//出現させようとしているものが赤コインか確認し、違うなら返す
		if (!temp->CheckTag(TagTable::RedCoin)) { 
			continue;
		}

		//呼び出しイベント名と持っているイベント名が一致する個体を呼び出す
		if (eventname == temp->eventName_)
		{
			if (temp->mActive) {
				temp->mActive = false;
			}
		}
	}
}
