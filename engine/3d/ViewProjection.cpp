#include "ViewProjection.h"
#include "Util.h"

std::unique_ptr<Camera> Camera::defaultCam = std::make_unique<Camera>();
Camera* Camera::camera = defaultCam.get();

void Camera::Initialize()
{
	eye = { 0, 0, -5 };		//���_���W
	target = {0, 0, 0};		//�����_���W
	up = { 0, 1, 0 };		//������x�N�g��
	UpdatematView();

	matProjection = Matrix4::PerspectiveFovLH(XMConvertToRadians(fovAngle), nearClip, farZ);
}

void Camera::UpdatematView()
{
	matView = Matrix4::LookAtLH(eye, target, up);
	
	matProjection = Matrix4::PerspectiveFovLH(XMConvertToRadians(fovAngle), nearClip, farZ);
}

void Camera::SetEye(Vector3 eye)
{
	this->eye = eye;
	UpdatematView();
}

void Camera::SetEyeXMF(XMFLOAT3 eye)
{
	this->eye.x = eye.x;
	this->eye.y = eye.y;
	this->eye.z = eye.z;
	UpdatematView();
}
