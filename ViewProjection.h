#pragma once
#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")
#include "DirectXInit.h"
#include "Vector3.h"
#include "Util.h"
#include <memory>

class Camera
{
public:

	XMMATRIX matView;
	XMFLOAT3 eye;	//���_���W
	XMFLOAT3 target;	//�����_���W
	XMFLOAT3 up;		//������x�N�g��

	float aspect = Util::window_width / Util::window_height;
	float nearClip = 0.1f;
	float farZ = 1000.0f;
	float fovAngle = 60;

	//�ˉe�ϊ��s��(�������e)
	XMMATRIX matProjection;

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

