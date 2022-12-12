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

class Camera
{
public:

	Matrix4 matView;
	Vector3 eye;	//視点座標
	Vector3 target;	//注視点座標
	Vector3 up;		//上方向ベクトル

	float aspect = Util::window_width / Util::window_height;
	float nearClip = 0.1f;
	float farZ = 1000.0f;
	float fovAngle = 60;

	//射影変換行列(投資投影)
	Matrix4 matProjection;

public:
	Camera()
	{
		Initialize();
	}
	void Initialize();
	void UpdatematView();

	void SetEye(Vector3 eye);
	void SetEyeXMF(XMFLOAT3 eye);

	static std::unique_ptr<Camera> defaultCam;
	static Camera *camera;

private:

};

