#include "CameraHitCheck.h"
#include "Player.h"
#include "PlayerCamera.h"

void CameraHitCheck::Initialize()
{
	SetModel(ModelManager::GetModel("Cube"));
	SetTexture(TextureManager::GetTexture("white"));
}

void CameraHitCheck::Update()
{
	Obj3d::Update(*Camera::sCamera);
}

void CameraHitCheck::Draw()
{
	Obj3d::Draw();
}
