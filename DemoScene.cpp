#include "DemoScene.h"
#include <ClearDrawScreen.h>
#include <Sprite.h>
#include <SceneManager.h>
#include "GameScene.h"

void DemoScene::Initialize()
{
	//3dオブジェクト用のパイプライン生成
	object3dPipelineSet = CreateObject3DPipeline();

	camera->Initialize();

	sphere.Initialize();
	sphere.SetModel(&ModelManager::GetInstance()->sphereM);
	sphere.SetTexture(&TextureManager::GetInstance()->white);
}

void DemoScene::Update()
{
	if (input->TriggerKey(DIK_K))
	{
		Game* game = new Game();
		sceneManager->SetScene(game);
		//sceneManager->ChangeScene("GAMEPLAY");
	}

	camera->UpdatematView();
	sphere.Update(*camera);
}

void DemoScene::Draw()
{
	BasicObjectPreDraw(object3dPipelineSet);

	sphere.Draw();

	//スプライトの前描画(共通コマンド)
	SpriteCommonBeginDraw(SpriteCommon::spriteCommon);

}

void DemoScene::End()
{

}