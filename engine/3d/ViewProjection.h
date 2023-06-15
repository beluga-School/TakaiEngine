#pragma once
#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")
#include "DirectXInit.h"
#include "Vector3.h"
#include "Util.h"
#include <memory>
#include "Matrix4.h"

class Obj3d;

class Camera
{
public:
	Matrix4 mMatView;
	Vector3 mEye;	//���_���W
	Vector3 mTarget;	//�����_���W
	Vector3 mUp;		//������x�N�g��

	float mAspect = Util::WIN_WIDTH / Util::WIN_HEIGHT;
	float mNearClip = 0.1f;
	float mFarZ = 10000.0f;
	float mFovAngle = 90;

	//�ˉe�ϊ��s��(�������e)
	Matrix4 mMatProjection;

public:
	Camera()
	{
		Initialize();
	}
	void Initialize();
	void UpdatematView();

	static std::unique_ptr<Camera> mDefaultCam;
	static Camera *sCamera;

private:
	float mDebugAngleX = 0;
	float mDebugAngleY = 0;
};

