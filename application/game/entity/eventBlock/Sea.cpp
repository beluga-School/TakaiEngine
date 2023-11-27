#include "Sea.h"
#include "ClearDrawScreen.h"

void Sea::LoadResource()
{
	TextureManager::Load("Resources\\sea.png", "sea");
}

void Sea::Initialize()
{
	SetModel(ModelManager::GetModel("Cube"));
	SetTexture(TextureManager::GetTexture("sea"));
}

void Sea::Update()
{
	flowTimer.Update();
	if (flowTimer.GetStarted() == false || flowTimer.GetEnd())
	{
		flowTimer.Start();
	}
	//オフセットを変更して水面を流す
	mTexOffset.x = flowTimer.GetTimeRate();
	mTexOffset.y = flowTimer.GetTimeRate();
	
	updownTimer.Update();
	if (updownTimer.GetStarted() == false || updownTimer.GetEnd())
	{
		updownTimer.Start();
		updown = !updown;
	}
	//上下に揺らして波っぽさを出す
	if (updown)
	{
		position.y = TEasing::lerp(initPos.y - updownRadius, initPos.y + updownRadius,updownTimer.GetTimeRate());
	}
	else
	{
		position.y = TEasing::lerp(initPos.y + updownRadius, initPos.y - updownRadius,updownTimer.GetTimeRate());
	}

	Obj3d::Update(*Camera::sCamera);
}

void Sea::Draw()
{
	BasicObjectPreDraw("GroundToon");
	Obj3d::Draw();
}

void Sea::SetInfo(const Vector3& position_, const Vector3& scale_, const Vector2& tiling)
{
	position = position_;
	scale = scale_;
	mTiling = tiling;

	initPos = position_;
}

float Sea::GetPosY()
{
	return position.y;
}
