#include "ViewProjection.h"
#include "Util.h"
#include <Input.h>
#include <TimeManager.h>

std::unique_ptr<Camera> Camera::defaultCam = std::make_unique<Camera>();
Camera* Camera::camera = defaultCam.get();

void Camera::Initialize()
{
	eye = { 0, 0, -5 };		//視点座標
	target = {0, 0, 0};		//注視点座標
	up = { 0, 1, 0 };		//上方向ベクトル
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

void Camera::DebugMove()
{
	float speed = 100.0f;

	if (Input::Get()->PushKey(DIK_D))
	{
		camera->eye.x += speed * TimeManager::deltaTime;
		camera->target.x += speed * TimeManager::deltaTime;
	}
	if (Input::Get()->PushKey(DIK_A))
	{
		camera->eye.x -= speed * TimeManager::deltaTime;
		camera->target.x -= speed * TimeManager::deltaTime;
	}

	if (Input::Get()->PushKey(DIK_W))
	{
		camera->eye.y += (speed / 2.f) * TimeManager::deltaTime;
	}
	if (Input::Get()->PushKey(DIK_S))
	{
		camera->eye.y -= (speed / 2.f) * TimeManager::deltaTime;
	}

	if (Input::Get()->PushKey(DIK_UP))
	{
		camera->eye.z += speed * TimeManager::deltaTime;
		camera->target.z += speed * TimeManager::deltaTime;
	}
	if (Input::Get()->PushKey(DIK_DOWN))
	{
		camera->eye.z -= speed * TimeManager::deltaTime;
		camera->target.z -= speed * TimeManager::deltaTime;
	}


	if (Input::Get()->PushKey(DIK_RIGHT))
	{
		camera->eye.x += speed * TimeManager::deltaTime;
	}
	if (Input::Get()->PushKey(DIK_LEFT))
	{
		camera->eye.x -= speed * TimeManager::deltaTime;
	}

	if (Input::Get()->PushKey(DIK_I))
	{
		camera->eye.y += speed * TimeManager::deltaTime;
		camera->target.y += speed * TimeManager::deltaTime;
	}
	if (Input::Get()->PushKey(DIK_K))
	{
		camera->eye.y -= speed * TimeManager::deltaTime;
		camera->target.y -= speed * TimeManager::deltaTime;
	}
}
