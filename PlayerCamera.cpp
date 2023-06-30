#include "PlayerCamera.h"
#include "Player.h"
#include "MathF.h"
#include "Input.h"
#include "Util.h"

using namespace Input;

void PlayerCamera::Initialize()
{
	Camera::sCamera->Initialize();
	Obj3d::Initialize();
}

void PlayerCamera::Update()
{
	//マウス固定
	Mouse::CurserLock(CurserLockState::LOCK);

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
	//縦回転
	mVerticalRad += MathF::AngleConvRad(Mouse::GetVelocity().y) * mSensitivity;
	//限界値を超えない処理
	if (mVerticalRad > MathF::PIf / 2 - MathF::AngleConvRad(1.0f)) mVerticalRad = MathF::PIf / 2 - MathF::AngleConvRad(1.0f);
	if (mVerticalRad < -MathF::PIf / 2 + MathF::AngleConvRad(1.0f)) mVerticalRad = -MathF::PIf / 2 + MathF::AngleConvRad(1.0f);
	rotation.x = mVerticalRad;

	//横回転
	mHorizontalRad += MathF::AngleConvRad(Mouse::GetVelocity().x) * mSensitivity;
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
