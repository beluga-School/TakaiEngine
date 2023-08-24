#include "PlayerCamera.h"
#include "Player.h"
#include "MathF.h"
#include "Input.h"
#include "Util.h"
#include "ImguiManager.h"
#include "Stage.h"
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

	switch (camMode)
	{
	case PlayerCamera::CamMode::Normal:
		NormalUpdate();
		break;
	case PlayerCamera::CamMode::StarGet:
		StarGetUpdate();
		break;
	}

	Camera::sCamera->UpdatematView();
	Obj3d::Update(*Camera::sCamera);

	CreateCamCol();
}

void PlayerCamera::Draw()
{
	Obj3d::Draw();
}

void PlayerCamera::BackTransparent()
{
	Player* player = Player::Get();

	//XとZをプレイヤーの後ろを円運動でいい感じに追従する
	Vector2 invObjNonY = MathF::CircularMotion({ player->position.x,player->position.z },
		(transparentObj.scale.x + transparentObj.scale.z) / 2 * 0.8f,
		-player->rotation.y - MathF::PIf / 2);

	//Yはカメラ位置と同じにする
	transparentObj.position = { invObjNonY.x,Camera::sCamera->mEye.y,invObjNonY.y };

	transparentObj.Update(*Camera::sCamera);

	//当たったオブジェクトの描画フラグを折る
	for (auto& obj : StageChanger::Get()->mEntitys)
	{
		//ブロック以外なら次へ
		if (!obj.CheckTag(TagTable::Block))continue;
		
		//ブロックなのが確定したのでブロック型に変換
		Block* block = static_cast<Block*>(&obj);

		//当たってたら消える
		if (Collsions::BoxColAABB(block->box, transparentObj))
		{
			//地面が透けてほしくないので、地面の座標がプレイヤーの足元より下なら
			//当たってても透ける処理をスキップする
			if (block->box.position.y + block->box.scale.y / 2 <= player->GetFeet())
			{
				continue;
			}

			if (block->transparentTimer.GetStarted() == false)
			{
				block->transparentTimer.Start();
			}
			else if (block->transparentTimer.GetReverseStarted())
			{
				block->transparentTimer.Reset();
			}
			//アウトラインはすぐに消す
			block->SetOutLineAlpha(0.0f);
		}
		//当たってないなら段々濃くする
		else
		{
			//タイマーのTimeRateが0.0~1.0の範囲で動くので、その値を反転させたものをAlphaとして扱う
			block->transparentTimer.ReverseStart();
			//アウトラインはすぐに戻す
			block->SetOutLineAlpha(1.0f);
		}
		//段々薄くしたり濃くしたりする
		block->color_.w = 1.0f - block->transparentTimer.GetTimeRate();
	}
}

void PlayerCamera::ChangeNormalMode()
{
	camMode = CamMode::Normal;

	camMoveTimer.Reset();
	mRadius = saveRadius;
}

void PlayerCamera::ChangeStarGetMode()
{
	//すでに開始されてるならスキップ
	if (camMode == CamMode::StarGet)return;

	Player* player = Player::Get();

	camMode = CamMode::StarGet;
	camMoveTimer.Start();
	radiusMoveTimer.Reset();

	//現在位置を始点に
	starGetCamPosS = Camera::sCamera->mEye;

	//プレイヤーの正面座標を算出
	mCenterVec = Player::Get()->matWorld.ExtractAxisZ();
	//y方向を反対にして上に行くように
	mCenterVec.y = 0.25f;

	position = player->position;

	//プレイヤーの正面座標を終点に
	starGetCamPosE = position + (mCenterVec * mRadius);

	saveRadius = mRadius;
};

void PlayerCamera::StarGetUpdate()
{
	Player* player = Player::Get();

	camMoveTimer.Update();
	radiusMoveTimer.Update();

	Camera::sCamera->mTarget = position;

	Camera::sCamera->mEye = TEasing::InQuad(starGetCamPosS, starGetCamPosE, camMoveTimer.GetTimeRate());

	if (camMoveTimer.GetEnd())
	{
		if(radiusMoveTimer.GetStarted() == false)radiusMoveTimer.Start();
		mRadius = TEasing::InQuad(saveRadius,4.0f, radiusMoveTimer.GetTimeRate());

		position = player->position;

		Camera::sCamera->mEye = position + (mCenterVec * mRadius);
	}
}

void PlayerCamera::NormalUpdate()
{
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
