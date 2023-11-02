#include "Camera.h"
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
}