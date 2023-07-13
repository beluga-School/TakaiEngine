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
	//���łɃJ�������[�h���ς���Ă���΃X�L�b�v
	if (PlayerCamera::Get()->GetCamMode() == PlayerCamera::CamMode::StarGet)return;
	
	//�J�������X�^�[���莞�̃J�����ɕύX
	PlayerCamera::Get()->ChangeStarGetMode();
}
