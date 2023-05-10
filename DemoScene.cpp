#include "DemoScene.h"
#include <ClearDrawScreen.h>
#include <Sprite.h>

void DemoScene::Initialize()
{
	//3d�I�u�W�F�N�g�p�̃p�C�v���C������
	object3dPipelineSet = CreateObject3DPipeline();

	camera->Initialize();

	sphere.Initialize();
	sphere.SetModel(&ModelManager::GetInstance()->sphereM);
	sphere.SetTexture(&TextureManager::GetInstance()->white);
}

void DemoScene::Update()
{
	camera->UpdatematView();
	sphere.Update(*camera);
}

void DemoScene::Draw()
{
	BasicObjectPreDraw(object3dPipelineSet);

	sphere.Draw();

	//�X�v���C�g�̑O�`��(���ʃR�}���h)
	SpriteCommonBeginDraw(SpriteCommon::spriteCommon);

}

void DemoScene::End()
{

}