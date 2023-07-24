#include "DebugCamera.h"
#include <Input.h>
#include <MathF.h>
#include <ImguiManager.h>

using namespace Input;

void DebugCamera::Initialize()
{
	Obj3d::Initialize();
	Camera::sCamera->Initialize();
}

void DebugCamera::Update()
{
	//ƒ}ƒEƒXŒÅ’è‚ð‰ðœ
	Mouse::CurserLock(CurserLockState::UNLOCK);

	mCenterVec = matWorld.ExtractAxisZ();
	mSideVec = matWorld.ExtractAxisX();

	int16_t inputCenter = (Keyboard::PushKey(DIK_DOWN) - Keyboard::PushKey(DIK_UP));
	position.x += inputCenter * mCenterVec.x * MOVE_SPEED;
	position.y += inputCenter * mCenterVec.y * MOVE_SPEED;
	position.z += inputCenter * mCenterVec.z * MOVE_SPEED;

	int16_t inputSide = (Keyboard::PushKey(DIK_LEFT) - Keyboard::PushKey(DIK_RIGHT));
	position.x += inputSide * mSideVec.x * MOVE_SPEED;
	position.y += inputSide * mSideVec.y * MOVE_SPEED;
	position.z += inputSide * mSideVec.z * MOVE_SPEED;
	
	//À•W‚Ì’Ç]
	Camera::sCamera->mEye = position + (mCenterVec * mRadius);
	Camera::sCamera->mTarget = position;

	if (Mouse::Wheel() < 0)
	{
		mRadius += 2.0f;
	}
	if (Mouse::Wheel() > 0)
	{
		mRadius -= 2.0f;
	}

	mRadius = Util::Clamp(mRadius, 1.0f, 100.f);

	//‰ñ“]‚³‚¹‚éˆ—
	if (Mouse::Down(Click::MIDDLE))
	{
		mVerticalRad += MathF::AngleConvRad(Mouse::GetVelocity().y);
		mHorizontalRad += MathF::AngleConvRad(Mouse::GetVelocity().x);

		//ŒÀŠE’l‚ð’´‚¦‚È‚¢ˆ—
		if (mVerticalRad > MathF::PIf / 2 - MathF::AngleConvRad(1.0f)) mVerticalRad = MathF::PIf / 2 - MathF::AngleConvRad(1.0f);
		if (mVerticalRad < -MathF::PIf / 2 + MathF::AngleConvRad(1.0f)) mVerticalRad = -MathF::PIf / 2 + MathF::AngleConvRad(1.0f);

		rotation.x = mVerticalRad;
		rotation.y = mHorizontalRad;
	}

	Obj3d::Update(*Camera::sCamera);
	Camera::sCamera->UpdatematView();
}

void DebugCamera::SetRadius(float val)
{
	mRadius = val;
}

float DebugCamera::GetRadius()
{
	return mRadius;
}
