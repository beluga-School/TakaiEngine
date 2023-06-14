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
	Vector3 eye;	//視点座標
	Vector3 target;	//注視点座標
	Vector3 up;		//上方向ベクトル

	float aspect = Util::window_width / Util::window_height;
	float nearClip = 0.1f;
	float farZ = 10000.0f;
	float fovAngle = 90;

	//射影変換行列(投資投影)
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

