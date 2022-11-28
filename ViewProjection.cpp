#include "ViewProjection.h"
#include "Util.h"

std::unique_ptr<Camera> Camera::defaultCam = std::make_unique<Camera>();
Camera* Camera::camera = defaultCam.get();

void Camera::Initialize()
{
	eye = { 0, 0, -5 };	//視点座標
	target = {0, 0, 0};		//注視点座標
	up = { 0, 1, 0 };		//上方向ベクトル
	UpdatematView();

	matProjection = XMMatrixPerspectiveFovLH(XMConvertToRadians(fovAngle), aspect, nearClip, farZ);
}

void Camera::UpdatematView()
{
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
	
	matProjection = XMMatrixPerspectiveFovLH(XMConvertToRadians(fovAngle), aspect, nearClip, farZ);
}

void Camera::SetEye(Vector3 eye)
{
	XMFLOAT3 dxeye;
	dxeye.x = eye.x;
	dxeye.y = eye.y;
	dxeye.z = eye.z;
	this->eye = dxeye;
	UpdatematView();
}

void Camera::SetEyeXMF(XMFLOAT3 eye)
{
	this->eye = eye;
	UpdatematView();
}
