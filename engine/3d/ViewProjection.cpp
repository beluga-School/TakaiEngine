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
	mEye = { 0, 0, -5 };		//視点座標
	mTarget = {0, 0, 0};		//注視点座標
	mUp = { 0, 1, 0 };		//上方向ベクトル
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