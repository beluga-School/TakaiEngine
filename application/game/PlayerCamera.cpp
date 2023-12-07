#include "PlayerCamera.h"
#include "Player.h"
#include "MathF.h"
#include "Input.h"
#include "Util.h"
#include "ImguiManager.h"
#include "StageChanger.h"
#include "Star.h"
#include "EventManager.h"

using namespace Input;

void PlayerCamera::Initialize()
{
	Camera::sCamera->Initialize();
	Obj3d::Initialize();

	transparentObj.Initialize();
	transparentObj.SetModel(ModelManager::GetModel("BlankCube"));
	transparentObj.scale = { 5,5,5 };
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

	NormalUpdate();

	Camera::sCamera->UpdatematView();
	Obj3d::Update(*Camera::sCamera);

	CreateCamCol();
}

void PlayerCamera::Draw()
{
	Obj3d::Draw();

	transparentObj.Draw();
}

void PlayerCamera::RadiusChange()
{
	radiusMoveTimer.Update();

	if (radiusMoveTimer.GetRun())
	{
		mRadius = TEasing::InQuad(startRadius, endRadius, radiusMoveTimer.GetTimeRate());
	}

	if (Player::Get()->CheckState(Player::PlayerState::Debug))
	{
		//ラディウス変更(消してもいいかも)
		if (Mouse::Wheel() < 0)
		{
			mRadius += 2.0f;
		}
		if (Mouse::Wheel() > 0)
		{
			mRadius -= 2.0f;
		}
	}

	mRadius = Util::Clamp(mRadius, 1.0f, 30.f);
}

void PlayerCamera::NormalUpdate()
{
	Player* player = Player::Get();

	mCenterVec = matWorld.ExtractAxisZ();
	position = player->position;
	position.y += offsetY;

	Camera::sCamera->mEye = position - (mCenterVec * mRadius);
	Camera::sCamera->mTarget = position;

	RadiusChange();

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

	//限界値を超えない処理
	if (mVerticalRad > MathF::PIf / 2 - MathF::AngleConvRad(1.0f)) mVerticalRad = MathF::PIf / 2 - MathF::AngleConvRad(1.0f);
	if (mVerticalRad < -MathF::PIf / 2 + MathF::AngleConvRad(1.0f)) mVerticalRad = -MathF::PIf / 2 + MathF::AngleConvRad(1.0f);

	PlayerFollow();
}

void PlayerCamera::CreateCamCol()
{
	Player* player = Player::Get();

	//カメラ座標を当たり判定用に保存
	//カメラ位置とプレイヤー位置の中心を取る
	cameraCol.position = {
		(Camera::sCamera->mEye.x + player->position.x) / 2,
		(Camera::sCamera->mEye.y + player->position.y) / 2,
		(Camera::sCamera->mEye.z + player->position.z) / 2,
	};

	//中心位置から元座標へ引き算して間の長さを出す
	//XとZの幅の出し方がわかんないので適当な大きさを入れておく
	cameraCol.scale = { player->scale.x,
		Util::Abs(cameraCol.position.y - Camera::sCamera->mEye.y),
		player->scale.z };

	cameraCol.scale.y = Util::Clamp(cameraCol.scale.y, player->scale.y, 9999.f);
}

void PlayerCamera::PlayerFollow()
{
	//yの処理
	rotation.y = mHorizontalRad;
	//xの処理
	rotation.x = mVerticalRad;
}

void PlayerCamera::CheckDebug()
{
	ImGui::SliderFloat("offsetY %f", &offsetY,0.0f,10.0f);
}

void PlayerCamera::ChangeRadius(float radius, float time)
{
	startRadius = mRadius;
	endRadius = mRadius + radius;

	radiusMoveTimer.mMaxTime = time;
	if (!radiusMoveTimer.GetRun())
	{
		radiusMoveTimer.Start();
	}
}

void PlayerCamera::ChangeFixingRadius(float radius, float time)
{
	startRadius = mRadius;
	endRadius = radius;

	radiusMoveTimer.mMaxTime = time;
	if (!radiusMoveTimer.GetRun())
	{
		radiusMoveTimer.Start();
	}
}

void PlayerCamera::ChangeDefaultRadius(float radius)
{
	plusRadius = radius - DEFAULT_RADIUS;
}

float PlayerCamera::GetDefRadius()
{
	return DEFAULT_RADIUS + plusRadius;
}

void PlayerCamera::InitRadius()
{
	startRadius = mRadius;
	endRadius = GetDefRadius();

	radiusMoveTimer.mMaxTime = 0.5f;
	if (!radiusMoveTimer.GetRun())
	{
		radiusMoveTimer.Start();
	}
}