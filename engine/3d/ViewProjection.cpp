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
	mEye = { 0, 0, -5 };		//���_���W
	mTarget = {0, 0, 0};		//�����_���W
	mUp = { 0, 1, 0 };		//������x�N�g��
	UpdatematView();

	mMatProjection = Matrix4::PerspectiveFovLH(DirectX::XMConvertToRadians(mFovAngle), mNearClip, mFarZ);
}

void Camera::UpdatematView()
{
	mMatView = Matrix4::LookAtLH(mEye, mTarget, mUp);

	mMatProjection = Matrix4::PerspectiveFovLH(DirectX::XMConvertToRadians(mFovAngle), mNearClip, mFarZ);

	////�V�F�C�N��Ɉʒu�����ɖ߂�
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