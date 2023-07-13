#include "Star.h"
#include "PlayerCamera.h"

void Star::Initialize()
{
	SetModel(ModelManager::GetModel("star"));
	SetTexture(TextureManager::GetTexture("white"));
}

void Star::Update()
{
	Obj3d::Update(*Camera::sCamera);
}

void Star::Draw()
{
	Obj3d::DrawMaterial();
}

void Star::HitEffect()
{
	//すでにカメラモードが変わっていればスキップ
	if (PlayerCamera::Get()->GetCamMode() == PlayerCamera::CamMode::StarGet)return;
	
	//カメラをスター入手時のカメラに変更
	PlayerCamera::Get()->ChangeStarGetMode();
}
