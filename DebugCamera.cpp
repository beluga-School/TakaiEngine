#include "DebugCamera.h"
#include <Input.h>
#include <MathF.h>
#include <ImguiManager.h>

using namespace Input;

void DebugCamera::Initialize()
{
	Obj3d::Initialize();
	Camera::mCamera->Initialize();
}

void DebugCamera::Update()
{
	mCenterVec = matWorld.ExtractAxisZ();
	mSideVec = matWorld.ExtractAxisX();

	int16_t inputCenter = (Keyboard::PushKey(DIK_DOWN) - Keyboard::PushKey(DIK_UP));
	position.x += inputCenter * mCenterVec.x * mMoveSpeed;
	position.y += inputCenter * mCenterVec.y * mMoveSpeed;
	position.z += inputCenter * mCenterVec.z * mMoveSpeed;

	int16_t inputSide = (Keyboard::PushKey(DIK_LEFT) - Keyboard::PushKey(DIK_RIGHT));
	position.x += inputSide * mSideVec.x * mMoveSpeed;
	position.y += inputSide * mSideVec.y * mMoveSpeed;
	position.z += inputSide * mSideVec.z * mMoveSpeed;
	
	//À•W‚Ì’Ç]
	Camera::mCamera->mEye = position + (mCenterVec * mRadius);
	Camera::mCamera->mTarget = position;

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

	Obj3d::Update(*Camera::mCamera);
	Camera::mCamera->UpdatematView();
}
