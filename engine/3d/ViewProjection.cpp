#include "ViewProjection.h"
#include "Util.h"
#include <Input.h>
#include <TimeManager.h>
#include <MathF.h>
#include <ImguiManager.h>

std::unique_ptr<Camera> Camera::sDefaultCam = std::make_unique<Camera>();
Camera* Camera::sCamera = sDefaultCam.get();

void Camera::Initialize()
{
	mEye = { 0, 0, -5 };		//���_���W
	mTarget = {0, 0, 0};		//�����_���W
	mUp = { 0, 1, 0 };		//������x�N�g��
	UpdatematView();

	mMatProjection = Matrix4::PerspectiveFovLH(XMConvertToRadians(mFovAngle), mNearClip, mFarZ);
}

void Camera::UpdatematView()
{
	mMatView = Matrix4::LookAtLH(mEye, mTarget, mUp);

	mMatProjection = Matrix4::PerspectiveFovLH(XMConvertToRadians(mFovAngle), mNearClip, mFarZ);
}

//void Camera::SetEye(Vector3 eye)
//{
//	this->eye = eye;
//	UpdatematView();
//}
//
//void Camera::SetEyeXMF(XMFLOAT3 eye)
//{
//	this->eye.x = eye.x;
//	this->eye.y = eye.y;
//	this->eye.z = eye.z;
//	UpdatematView();
//}