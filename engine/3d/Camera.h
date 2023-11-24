#pragma once
#include "DirectXInit.h"
#include "Vector3.h"
#include "Util.h"
#include <memory>
#include "Matrix4.h"

//前方宣言
class Obj3d;

/*! Camera
	3D空間上に配置するカメラ
*/
class Camera
{
public:
	Matrix4 mMatView;
	Vector3 mEye;		//視点座標
	Vector3 mTarget;	//注視点座標
	Vector3 mUp;		//上方向ベクトル

	float mAspect = Util::WIN_WIDTH / Util::WIN_HEIGHT;
	float mNearClip = 0.1f;
	float mFarZ = 10000.0f;
	float mFovAngle = 90;

	//射影変換行列(投資投影)
	Matrix4 mMatProjection;

public:
	Camera()
	{
		Initialize();
	}
	void Initialize();
	void UpdatematView();

	static std::unique_ptr<Camera> sDefaultCam;
	static Camera *sCamera;

private:
	float mDebugAngleX = 0;
	float mDebugAngleY = 0;
};

