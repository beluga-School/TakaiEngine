#pragma once
#include "Camera.h"
#include "TEasing.h"
#include "Obj.h"
#include "Collision.h"

/*! PlayerCamera
	プレイヤーのポインタを取得し、それにsCameraを追従させる
*/
class PlayerCamera : public Obj3d
{
public:
	void Initialize();
	void Update();
	void Draw();

	static PlayerCamera* Get() {
		static PlayerCamera instance;
		return &instance;
	}

	float GetRadius() {
		return mRadius;
	};

	bool CamChangeEnd() {
		return radiusMoveTimer.GetEnd();
	};

	//Imguiに表示するステータスだけ関数にまとめた
	void CheckDebug();

	//現時点のラディウスから固定の値を何秒かけて引く
	void ChangeRadius(float radius,float time);

	//現時点のラディウスから固定の値まで変更する
	void ChangeFixingRadius(float radius,float time);

	//下のInitRadiusで移動する終点を決める
	void ChangeDefaultRadius(float radius);

	float GetDefRadius();

	void InitRadius();

private:
	PlayerCamera() {};
	~PlayerCamera() {};

	//通常の際のカメラ制御
	void NormalUpdate();

	//カメラの当たり判定を作成
	void CreateCamCol();

	//カメラがプレイヤーに後から追従する動きのまとめ
	void PlayerFollow();

	//ラディウス変更の更新
	void RadiusChange();

public:
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

private:
	//初期のカメラ距離を保存(初期設定用)
	const float DEFAULT_RADIUS = 8.0f;
	
	//初期のカメラ距離
	float mRadius = DEFAULT_RADIUS;

	//初期カメラ距離に追加するラディウス
	float plusRadius = 0;

	//このオブジェクトと当たったオブジェクトを透けさせる
	Obj3d transparentObj;

	//---スター取得時のカメラの挙動制御変数群
	//元のラディウスを保存
	float saveRadius = 0.0f;

	//イージングの始点、終点
	Vector3 starGetCamPosS{};
	Vector3 starGetCamPosE{};

	//カメラを動かすタイマー
	TEasing::easeTimer camRotaYTimer = 0.75f;

	float camMoveRotaCheck = 0.0f;
	float camMoveRotaCheckOffset = 1.0f;
	float oldCamRota = 0.0f;
	float startCamRota = 0.0f;

	float startRadius = 0;
	float endRadius = 0;
	//ラディウスを動かすタイマー
	TEasing::easeTimer radiusMoveTimer = 0.5f;

	float offsetY = 1.5f;
};

