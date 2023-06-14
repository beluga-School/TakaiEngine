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

	Matrix4 matView;
	Vector3 eye;	//���_���W
	Vector3 target;	//�����_���W
	Vector3 up;		//������x�N�g��

	float aspect = Util::window_width / Util::window_height;
	float nearClip = 0.1f;
	float farZ = 10000.0f;
	float fovAngle = 90;

	//�ˉe�ϊ��s��(�������e)
	Matrix4 matProjection;

public:
	Camera()
	{
		Initialize();
	}
	void Initialize();
	void UpdatematView();

	static std::unique_ptr<Camera> defaultCam;
	static Camera *camera;

private:
	float debugAngleX = 0;
	float debugAngleY = 0;
};

