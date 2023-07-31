#pragma once
#include "ViewProjection.h"
#include "TEasing.h"
#include "Obj.h"
#include "Collision.h"

//プレイヤーのポインタを取得し、それにsCameraを追従させる
class PlayerCamera : public Obj3d
{
public:
	void Initialize();
	void Update();
	void Draw();

	//カメラの後ろにあるオブジェクトを透けさせる処理
	void BackTransparent();

	static PlayerCamera* Get() {
		static PlayerCamera instance;
		return &instance;
	}

	//カメラの座標を保存
	Cube cameraCol;

	//縦回転
	float mVerticalRad = 0;
	//横回転
	float mHorizontalRad = 0;

	//カメラ回転の感度(マウス)
	float mMouseSensitivity = 0.15f;

	//カメラ回転の感度(パッド)
	float mPadSensitivity = 1.0f;

	//カメラベクトル
	Vector3 mCenterVec = { 0,0,0 };

	bool mouseLockChange = true;

	float GetRadius() {
		return mRadius;
	};

	enum class CamMode
	{
		Normal,
		StarGet,
	};

	CamMode GetCamMode()
	{
		return camMode;
	};

	bool CamChangeEnd() {
		return radiusMoveTimer.GetEnd();
	};

	void ChangeNormalMode();

	void ChangeStarGetMode();

private:
	PlayerCamera(){};
	~PlayerCamera(){};

	CamMode camMode = CamMode::Normal;

	//スター取得時のカメラ制御
	void StarGetUpdate();

	//通常の際のカメラ制御
	void NormalUpdate();

	//カメラの当たり判定を作成
	void CreateCamCol();

	//初期のカメラ距離
	float mRadius = 8.0f;

	//このオブジェクトと当たったオブジェクトを透けさせる
	Obj3d transparentObj;

	//---スター取得時のカメラの挙動制御変数群
	//元のラディウスを保存
	float saveRadius = 0.0f;

	//イージングの始点、終点
	Vector3 starGetCamPosS{};
	Vector3 starGetCamPosE{};

	//カメラを動かすタイマー
	TEasing::easeTimer camMoveTimer = 1.0f;

	//ラディウスを動かすタイマー
	TEasing::easeTimer radiusMoveTimer = 0.5f;
};

