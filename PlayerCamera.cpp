#include "PlayerCamera.h"
#include "Player.h"
#include "MathF.h"
#include "Input.h"
#include "Util.h"
#include "ImguiManager.h"

using namespace Input;

void PlayerCamera::Initialize()
{
	Camera::sCamera->Initialize();
	Obj3d::Initialize();
}

void PlayerCamera::Update()
{
	//マウス固定
	if (Input::Keyboard::TriggerKey(DIK_N))
	{
		mouseLockChange = !mouseLockChange;
	}
	if (mouseLockChange)
	{
		Mouse::CurserLock(CurserLockState::LOCK);
	}
	else
	{
		Mouse::CurserLock(CurserLockState::UNLOCK);
	}

	Player* player = Player::Get();

	mCenterVec = matWorld.ExtractAxisZ();
	position = player->position;

	Camera::sCamera->mEye = position - (mCenterVec * mRadius);
	Camera::sCamera->mTarget = position;

	//ラディウス変更(消してもいいかも)
	if (Mouse::Wheel() < 0)
	{
		mRadius += 2.0f;
	}
	if (Mouse::Wheel() > 0)
	{
		mRadius -= 2.0f;
	}

	mRadius = Util::Clamp(mRadius, 1.0f, 30.f);

	//回転させる処理
	if (Input::Pad::CheckConnectPad())
	{
		//縦回転
		mVerticalRad += MathF::AngleConvRad(Pad::GetRStickMove().y) * mPadSensitivity;

		//横回転
		mHorizontalRad += MathF::AngleConvRad(Pad::GetRStickMove().x) * mPadSensitivity;
	}

	//マウス固定されてるなら
	if (mouseLockChange)
	{
		//縦回転
		mVerticalRad += MathF::AngleConvRad(Mouse::GetVelocity().y) * mMouseSensitivity;

		//横回転
		mHorizontalRad += MathF::AngleConvRad(Mouse::GetVelocity().x) * mMouseSensitivity;
	}

	//後で両方同時に動かしたときに加速しないようにclampする

	//限界値を超えない処理
	if (mVerticalRad > MathF::PIf / 2 - MathF::AngleConvRad(1.0f)) mVerticalRad = MathF::PIf / 2 - MathF::AngleConvRad(1.0f);
	if (mVerticalRad < -MathF::PIf / 2 + MathF::AngleConvRad(1.0f)) mVerticalRad = -MathF::PIf / 2 + MathF::AngleConvRad(1.0f);

	rotation.x = mVerticalRad;
	rotation.y = mHorizontalRad;

	Camera::sCamera->UpdatematView();
	Obj3d::Update(*Camera::sCamera);

	//xはキャラクターの回転と一緒に動かす
	//yはカメラオブジェクトで動かす
	//shift押しながらならxもカメラオブジェクトで動かす
}

void PlayerCamera::Draw()
{
	Obj3d::Draw();
}
