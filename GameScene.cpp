#include "GameScene.h"
#include "TimeManager.h"

void GameScene::Initialize()
{
	//3dオブジェクト用のパイプライン生成
	object3dPipelineSet = CreateObject3DPipeline();

	//ジオメトリオブジェクト用パイプライン生成
	geometryObjectPipelineSet = CreateGeometryPipeline();

	//スプライト
	SpriteCreate(&pizzaSprite, &TextureManager::GetInstance()->pizza, { 0.5f,0.5f });
	SpriteInit(pizzaSprite, SpriteCommon::spriteCommon, { 100,100 }, 0);
	SpriteSetSize(pizzaSprite, { 100,100 });
	
	SpriteCreate(&slimeSprite, &TextureManager::GetInstance()->slime, { 0.5f,0.5f });
	SpriteInit(slimeSprite, SpriteCommon::spriteCommon, { 100,100 }, 0, { 1,1,1,1 });

	//ジオメトリオブジェクト生成
	particleManager->Initialize();

	skydome.Initialize();

	//どのモデルの形を使うかを設定
	skydome.SetModel(&ModelManager::GetInstance()->skyDomeM);

	skydome.SetTexture(&TextureManager::GetInstance()->slime);

	//ビュー変換行列(透視投影)を計算
	camera->Initialize();

	daruma.Initialize();
	daruma.SetModel(&ModelManager::GetInstance()->darumaM);
	daruma.SetTexture(&TextureManager::GetInstance()->white);

	billboard.Initialize();
	billboard.SetModel(&ModelManager::GetInstance()->boardM);
	billboard.SetTexture(&TextureManager::GetInstance()->pizza);

	billboard.position = { 10,10,10 };
	billboard.scale = { 3,3,3 };

	camera->target = {
		0,0,0
	};

	camera->SetEye({ 0,20,-10 });

	stage.Initialize();
	stage.position = { 0,-10,0 };
	stage.scale = { 10,10,10 };
}

void GameScene::Update()
{
	//カメラ座標を動かす
	if (input->PushKey(DIK_RIGHT)) {
		camera->eye.x += 20 * TimeManager::deltaTime;
	}
	if (input->PushKey(DIK_LEFT)) {
		camera->eye.x -= 20 * TimeManager::deltaTime;
	}
	if (input->PushKey(DIK_UP)) {
		camera->eye.y += 20 * TimeManager::deltaTime;
	}
	if (input->PushKey(DIK_DOWN)) {
		camera->eye.y -= 20 * TimeManager::deltaTime;
	}

	if (input->PushKey(DIK_D)) {

		camera->eye.x += 20 * TimeManager::deltaTime;
		camera->target.x += 20 * TimeManager::deltaTime;
	}
	if (input->PushKey(DIK_A)) {

		camera->eye.x -= 20 * TimeManager::deltaTime;
		camera->target.x -= 20 * TimeManager::deltaTime;
	}
	if (input->PushKey(DIK_W)) {

		camera->eye.z += 20 * TimeManager::deltaTime;
		camera->target.z += 20 * TimeManager::deltaTime;
	}
	if (input->PushKey(DIK_S)) {

		camera->eye.z -= 20 * TimeManager::deltaTime;
		camera->target.z -= 20 * TimeManager::deltaTime;
	}

	camera->UpdatematView();

	if (input->TriggerKey(DIK_R))
	{
		camera->Initialize();
		camera->eye.y = 20.0f;
		camera->UpdatematView();
	}

	camera->UpdatematView();

	//オブジェクトの更新
	particleManager->Update(camera->matView, camera->matProjection);

	skydome.Update(camera->matView, camera->matProjection);

	daruma.Update(camera->matView, camera->matProjection);
	billboard.Update(camera->matProjection);

	stage.Update(camera->matView, camera->matProjection);
}

void GameScene::Draw()
{
	BasicObjectPreDraw(object3dPipelineSet);

	//描画処理

	stage.Draw();

	billboard.Draw();
	daruma.DrawMaterial();

	skydome.DrawMaterial();

	GeometryObjectPreDraw(geometryObjectPipelineSet);
	//particleManager->Draw(&TextureManager::GetInstance()->particle);


	//スプライトの前描画(共通コマンド)
	SpriteCommonBeginDraw(SpriteCommon::spriteCommon);

	//スプライト単体描画
	//SpriteDraw(pizzaSprite);
	SpriteDraw(slimeSprite);
}

void GameScene::End()
{
}
