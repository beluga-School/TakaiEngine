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

private:
	PlayerCamera(){};
	~PlayerCamera(){};

	float mRadius = 5.0f;

	//このオブジェクトと当たったオブジェクトを透けさせる
	Obj3d transparentObj;
};

