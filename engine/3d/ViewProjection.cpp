#include "ViewProjection.h"
#include "Util.h"
#include <Input.h>
#include <TimeManager.h>
#include <MathF.h>
#include <ImguiManager.h>
#include <DirectXMath.h>
#include <MathF.h>

std::unique_ptr<Camera> Camera::sDefaultCam = std::make_unique<Camera>();
Camera* Camera::sCamera = sDefaultCam.get();

void Camera::Initialize()
{
	mEye = { 0, 0, -5 };		//視点座標
	mTarget = {0, 0, 0};		//注視点座標
	mUp = { 0, 1, 0 };		//上方向ベクトル
	UpdatematView();

	mMatProjection = Matrix4::PerspectiveFovLH(DirectX::XMConvertToRadians(mFovAngle), mNearClip, mFarZ);
}

void Camera::UpdatematView()
{
	mMatView = Matrix4::LookAtLH(mEye, mTarget, mUp);

	mMatProjection = Matrix4::PerspectiveFovLH(DirectX::XMConvertToRadians(mFovAngle), mNearClip, mFarZ);

	////シェイク後に位置を元に戻す
	//mEye.x = savePos.x;
	//mEye.y = savePos.y;
}

//void Camera::CameraShake(float power)
//{
//	Vector2 randomVec = {
//		MathF::GetRand(-1,1),
//		MathF::GetRand(-1,1)
//	};
//
//	Vector2 shakePos = {
//		mEye.x + randomVec.x * power,
//		mEye.y + randomVec.y * power
//	};
//
//	savePos = { mEye.x,mEye.y};
//
//	mEye.x = shakePos.x;
//	mEye.y = shakePos.y;
//}

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