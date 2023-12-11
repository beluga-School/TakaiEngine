#include "RedCoin.h"
#include "StageChanger.h"

void RedCoin::Initialize()
{
	SetModel(ModelManager::GetModel("Coin"));
	SetTexture(TextureManager::GetTexture("white"));
	color_ = { 1,0,0,1 };
}

void RedCoin::Update()
{
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

	hit = true;
}

void RedCoin::EventPopRedCoin(const std::string& eventname)
{
	for (auto& coin : StageChanger::Get()->mEntitys)
	{
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
