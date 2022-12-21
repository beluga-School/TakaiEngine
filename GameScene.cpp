#include "GameScene.h"
#include "TimeManager.h"
#include "Collision.h"

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
	//particleManager->Initialize();

	skydome.Initialize();

	//どのモデルの形を使うかを設定
	skydome.SetModel(&ModelManager::GetInstance()->skyDomeM);

	skydome.SetTexture(&TextureManager::GetInstance()->slime);

	//ビュー変換行列(透視投影)を計算
	camera->Initialize();

	daruma.Initialize();
	daruma.SetModel(&ModelManager::GetInstance()->darumaM);
	daruma.SetTexture(&TextureManager::GetInstance()->white);
	daruma.scale = { 5,5,5 };

	/*billboard.Initialize();
	billboard.SetModel(&ModelManager::GetInstance()->boardM);
	billboard.SetTexture(&TextureManager::GetInstance()->pizza);

	billboard.position = { 10,10,10 };
	billboard.scale = { 3,3,3 };*/

	camera->target = {
		0,0,0
	};

	camera->SetEye({ 0,10,-30 });

	stage.Initialize();
	stage.position = { 0,-10,0 };
	stage.scale = { 10,10,10 };

	board.Initialize();
	board.SetModel(&ModelManager::GetInstance()->boardM);
	board.scale = { 2,2,2 };
	board.position = { 0,10,0 };
}

void GameScene::Update()
{
	//カメラ座標を動かす
	if (input->PushKey(DIK_RIGHT)) {
		camera->eye.x += 20 * TimeManager::deltaTime;
		camera->target.x += 20 * TimeManager::deltaTime;
	}
	if (input->PushKey(DIK_LEFT)) {
		camera->eye.x -= 20 * TimeManager::deltaTime;
		camera->target.x -= 20 * TimeManager::deltaTime;
	}
	if (input->PushKey(DIK_UP)) {
		camera->eye.z += 20 * TimeManager::deltaTime;
		camera->target.z += 20 * TimeManager::deltaTime;
	}
	if (input->PushKey(DIK_DOWN)) {
		camera->eye.z -= 20 * TimeManager::deltaTime;
		camera->target.z -= 20 * TimeManager::deltaTime;
	}

	camera->UpdatematView();

	if (input->TriggerKey(DIK_R))
	{
		daruma.position = { 0,10,0 };
		daruma.jumpPower = 0;
	}

	camera->UpdatematView();

	//オブジェクトの更新
	//particleManager->Update(camera->matView, camera->matProjection);

	skydome.Update(camera->matView, camera->matProjection);

	stage.Update(camera->matView, camera->matProjection);
	stage.cubeCol.position = stage.position;
	stage.cubeCol.scale = stage.scale;
	stage.cubeCol.upPlane.normal = { 0,1,0.5f };

	if (RayPlaneCollision(daruma.rayCol, stage.cubeCol.upPlane))
	{
		daruma.onGround = true;
	}

	daruma.Update();

	board.Update(camera->matView, camera->matProjection);
}

void GameScene::Draw()
{
	BasicObjectPreDraw(object3dPipelineSet);

	//描画処理

	stage.Draw();

	//billboard.Draw();
	daruma.DrawMaterial();

	skydome.DrawMaterial();

	board.Draw();

	GeometryObjectPreDraw(geometryObjectPipelineSet);
	//particleManager->Draw(&TextureManager::GetInstance()->particle);


	//スプライトの前描画(共通コマンド)
	SpriteCommonBeginDraw(SpriteCommon::spriteCommon);

	//スプライト単体描画
	//SpriteDraw(pizzaSprite);
	//SpriteDraw(slimeSprite);
}

void GameScene::End()
{
}
