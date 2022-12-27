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
	skydome.SetModel(&ModelManager::GetInstance()->skyDomeM);
	skydome.SetTexture(&TextureManager::GetInstance()->slime);
	skydome.scale = { 10,10,10 };

	//ビュー変換行列(透視投影)を計算
	camera->Initialize();

	daruma.Initialize();
	gEnemy.Initialize();

	camera->target = {
		0,0,0
	};

	camera->SetEye({ 0,10,-30 });

	stage.SetStage1();
}

void GameScene::Update()
{
	CameraUpdate();

	if (input->PushKey(DIK_RIGHT))
	{
		daruma.rotation.y += 1 * TimeManager::deltaTime;
	}
	if (input->PushKey(DIK_LEFT))
	{
		daruma.rotation.y -= 1 * TimeManager::deltaTime;
	}

	if (input->TriggerKey(DIK_R))
	{
		daruma.position = { 0,10,0 };
		daruma.rotation = { 0,0,0 };
		daruma.jumpPower = 0;
	}

	//オブジェクトの更新
	//particleManager->Update(camera->matView, camera->matProjection);

	skydome.Update(camera->matView, camera->matProjection);

	stage.Update();

	stage.CheckMobCol(daruma);
	stage.CheckMobCol(gEnemy);

	daruma.Update();
	gEnemy.Update();
}

void GameScene::Draw()
{
	BasicObjectPreDraw(object3dPipelineSet);

	//描画処理

	stage.Draw();

	daruma.Draw();
	gEnemy.Draw();

	skydome.DrawMaterial();

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

int upCam = 0;
void GameScene::CameraUpdate()
{
	if (input->TriggerKey(DIK_1))
	{
		upCam = 1;
	}
	if (input->TriggerKey(DIK_2))
	{
		upCam = 2;
	}
	if (input->TriggerKey(DIK_0))
	{
		upCam = 0;
	}

	Vector3 cVec = {0,0,0};
	switch (upCam)
	{
	case 0:
		//カメラ座標をプレイヤーに追従
		cVec = daruma.centerVec * 20;
		camera->eye = daruma.position + cVec;
		camera->eye.y += 10;
		camera->target = daruma.position;
		camera->target.y += 5;
		break;
	case 1:
		camera->eye = daruma.position;
		camera->eye.z += 1;
		camera->eye.y += 35;
		camera->target = daruma.position;
		break;
	case 2:
		camera->eye = daruma.position;
		camera->eye.z += 35;
		camera->target = daruma.position;
		break;
	}

	camera->UpdatematView();
}
