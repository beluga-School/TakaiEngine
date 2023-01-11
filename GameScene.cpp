#include "GameScene.h"
#include "TimeManager.h"
#include "Collision.h"
#include "ImguiManager.h"
#include "MathF.h"
#include "ObjParticle.h"

void Game::Initialize()
{
	//3dオブジェクト用のパイプライン生成
	object3dPipelineSet = CreateObject3DPipeline();

	//ジオメトリオブジェクト用パイプライン生成
	geometryObjectPipelineSet = CreateGeometryPipeline();

	//スプライト
	SpriteCreate(&sceneChangeBlockOut, &TextureManager::GetInstance()->white, { 0.5f,0.5f });
	SpriteInit(sceneChangeBlockOut, SpriteCommon::spriteCommon, { Util::window_width / 2,Util::window_height / 2 }, 0);
	SpriteSetSize(sceneChangeBlockOut, { Util::window_width,Util::window_height});
	sceneChangeBlockOut.color = { 0,0,0,1 };

	skydome.Initialize();
	skydome.SetModel(&ModelManager::GetInstance()->skyDomeM);
	skydome.SetTexture(&TextureManager::GetInstance()->white);
	skydome.scale = { 10,10,10 };
	skydome.color = { 0.05f,0.05f,0.05f,1.0f };

	SpriteCreate(&goalSprite, &TextureManager::GetInstance()->goalS, { 0.5f,0.5f });
	SpriteInit(goalSprite, SpriteCommon::spriteCommon, { Util::window_width / 2,Util::window_height / 2 }, 0);
	SpriteSetSize(goalSprite, { 1280,720 });
	
	SpriteCreate(&TitleSprite, &TextureManager::GetInstance()->titleTex, { 0.5f,0.5f });
	SpriteInit(TitleSprite, SpriteCommon::spriteCommon, { Util::window_width / 2,Util::window_height / 2 }, 0);
	SpriteSetSize(TitleSprite, { 1280,720 });

	SpriteCreate(&startSprite, &TextureManager::GetInstance()->startTex, { 0.5f,0.5f });
	SpriteInit(startSprite, SpriteCommon::spriteCommon, 
		{ Util::window_width / 2,Util::window_height / 2 + 100 }, 0);
	SpriteSetSize(startSprite, { 600,150 });
	
	SpriteCreate(&spaceSprite, &TextureManager::GetInstance()->spaceTex, { 0.5f,0.5f });
	SpriteInit(spaceSprite, SpriteCommon::spriteCommon,
		{ Util::window_width / 2,Util::window_height / 2 + 200 }, 0);
	SpriteSetSize(spaceSprite, { 600,150 });

	SpriteCreate(&dashIconSprite, &TextureManager::GetInstance()->dashIcon, { 0.5f,0.5f });
	SpriteInit(dashIconSprite, SpriteCommon::spriteCommon,
		{ Util::window_width -100,Util::window_height - 100 }, 0);
	SpriteSetSize(dashIconSprite, { 150,150 });
	
	SpriteCreate(&dashCoolSprite, &TextureManager::GetInstance()->dashCool, { 0.5f,0.5f });
	SpriteInit(dashCoolSprite, SpriteCommon::spriteCommon,
		{ Util::window_width -100,Util::window_height - 100 }, 0);
	SpriteSetSize(dashCoolSprite, { 150,150 });

	//ビュー変換行列(透視投影)を計算
	camera->Initialize();

	player.Initialize();
	player.position = player.spawnPos;

	goal.Initialize();
	goal.position = {0,350,130};
	goal.scale = { 10,10,10 };
	goal.texture = &TextureManager::GetInstance()->goalBlockTex;

	stage.SetStage1();

	SetAirEnemy({ 0,30,-190 });
	SetAirEnemy({ 0,30,-210 });
	SetAirEnemy({ 0,30,-230 });

	SetAirEnemy({ 80,110,-440 });
	SetAirEnemy({ 0,140,-380 });

	SetAirEnemy({ 0,260,-480 });
	SetAirEnemy({ 50,260,-480 });
	SetAirEnemy({ -50,260,-480 });

	SetGroundEnemy({ 0,30,-190 });
}

void Game::Update()
{


	switch (scene)
	{
	case Scene::Title:
		if (input->TriggerKey(DIK_SPACE))
		{
			SceneChange(Scene::Game);
			player.Respawn();
		}

		SpriteUpdate(TitleSprite, SpriteCommon::spriteCommon);
		SpriteUpdate(spaceSprite, SpriteCommon::spriteCommon);
		SpriteUpdate(dashCoolSprite, SpriteCommon::spriteCommon);

		break;
	case Scene::Game:
		if (player.isDead)
		{
			SceneChange(Scene::GameOver);
		}

		CameraUpdate();
		//オブジェクトの更新
		skydome.Update(camera->matView, camera->matProjection);

		stage.Update();
		goal.Update();

		player.Update(stage);

		gEnemyList.remove_if([](GroundEnemy& enemy) {
			return enemy.isDead;
			});
		for (GroundEnemy& gEnemy : gEnemyList)
		{
			gEnemy.Update(stage,player);
			if (CubeCollision(player.cubeCol, gEnemy.cubeCol))
			{
				gEnemy.isDead = true;
				for (int i = 0; i < 10; i++)
				{
					ParticleManager::GetInstance()->CreateCubeParticle(gEnemy.position, { 3,3,3 }, 20.0f, { 1.0f,1.0f,1.0f,1.0f });
				}
			}
		}

		airEnemyList.remove_if([](AirEnemy& enemy) {
			return enemy.isDead;
			});
		for (AirEnemy &airEnemy : airEnemyList)
		{
			airEnemy.Update(stage, player);
			for (Bullet& bullet : airEnemy.bullets)
			{
				bullet.Update();
				if (CubeCollision(bullet.cubeCol, player.cubeCol))
				{
					bullet.isDead = true;
					player.HitEffect();
				}
				for (const Block& block : stage.blockList)
				{
					if (CubeCollision(bullet.cubeCol, block.cubeCol))
					{
						bullet.isDead = true;
					}
				}
			}

			if (CubeCollision(player.cubeCol, airEnemy.cubeCol))
			{
				airEnemy.isDead = true;
				for (int i = 0; i < 10; i++)
				{
					ParticleManager::GetInstance()->CreateCubeParticle(airEnemy.position,{3,3,3}, 20.0f, { 1.0f,1.0f,1.0f,1.0f });
				}
			}
		}

		if (CubeCollision(player.cubeCol, goal.cubeCol))
		{
			SceneChange(Scene::Clear);
		}

		SpriteUpdate(dashIconSprite, SpriteCommon::spriteCommon);

		break;
	case Scene::Clear:
		if (input->TriggerKey(DIK_SPACE))
		{
			SceneChange(Scene::Title);
			player.Respawn();
		}

		SpriteUpdate(goalSprite, SpriteCommon::spriteCommon);

		break;
	case Scene::GameOver:
		if (input->TriggerKey(DIK_SPACE))
		{
			SceneChange(Scene::Game);
			player.Respawn();
		}
		break;
	}

	SpriteUpdate(sceneChangeBlockOut,SpriteCommon::spriteCommon);
	ParticleManager::GetInstance()->Update();

	SceneChangeUpdate();
}

void Game::Draw()
{
	switch (scene)
	{
	case Scene::Title:
		BasicObjectPreDraw(object3dPipelineSet);
		

		GeometryObjectPreDraw(geometryObjectPipelineSet);

		//スプライトの前描画(共通コマンド)
		SpriteCommonBeginDraw(SpriteCommon::spriteCommon);

		SpriteDraw(TitleSprite);
		SpriteDraw(startSprite);
		SpriteDraw(spaceSprite);
		break;
	case Scene::Game:
		BasicObjectPreDraw(object3dPipelineSet);

		//描画処理
		stage.Draw();

		goal.Draw();

		player.Draw();
		for (GroundEnemy& gEnemy : gEnemyList)
		{
			gEnemy.DrawMaterial();
		}

		for (AirEnemy& airEnemy : airEnemyList)
		{
			airEnemy.Draw();
		}

		skydome.Draw();

		ParticleManager::GetInstance()->Draw();

		GeometryObjectPreDraw(geometryObjectPipelineSet);

		//スプライトの前描画(共通コマンド)
		SpriteCommonBeginDraw(SpriteCommon::spriteCommon);
		SpriteDraw(dashIconSprite);
		if (player.dashCool > 0)
		{
			SpriteDraw(dashCoolSprite);
		}

		break;
	case Scene::Clear:
		BasicObjectPreDraw(object3dPipelineSet);

		GeometryObjectPreDraw(geometryObjectPipelineSet);

		//スプライトの前描画(共通コマンド)
		SpriteCommonBeginDraw(SpriteCommon::spriteCommon);
		SpriteDraw(goalSprite);


		break;
	case Scene::GameOver:

		break;
	}

	SpriteCommonBeginDraw(SpriteCommon::spriteCommon);
	SpriteDraw(sceneChangeBlockOut);

}

void Game::End()
{
	ParticleManager::GetInstance()->AllDelete();
}

int upCam = 0;
float mag = 20;

void Game::CameraUpdate()
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
		cVec = player.centerVec * mag;
		camera->eye = player.position + cVec;
		camera->eye.y += 15;
		camera->target = player.position;
		camera->target.y += 10;
		if (player.isDash)
		{
			if (mag < 40)mag += 20 * TimeManager::deltaTime;
		}
		else
		{
			if (mag > 20)mag -= 20 * TimeManager::deltaTime;
		}
		//mag = MathF::Complement(mag2, mag3, 3);

		break;
	case 1:
		camera->eye = player.position;
		camera->eye.z += 1;
		camera->eye.y += 35;
		camera->target = player.position;
		break;
	case 2:
		camera->eye = player.position;
		camera->eye.z += 35;
		camera->target = player.position;
		break;
	}

	camera->UpdatematView();
}

void Game::SceneChange(Scene scene)
{
	nextScene = scene;
}

void Game::SceneChangeUpdate()
{
	if (scene != nextScene)
	{
		sceneChangeBlockOut.color.w += TimeManager::deltaTime;
		if (sceneChangeBlockOut.color.w >= 1)
		{
			sceneChangeBlockOut.color.w = 1;
 			scene = nextScene;
		}
	}
	else
	{
		sceneChangeBlockOut.color.w -= TimeManager::deltaTime;
		if (sceneChangeBlockOut.color.w <= 0)
		{
			sceneChangeBlockOut.color.w = 0;
		}
	}
}

void Game::SetAirEnemy(Vector3 position)
{
	airEnemyList.emplace_back();
	airEnemyList.back().Initialize(position);
}

void Game::SetGroundEnemy(Vector3 position)
{
	gEnemyList.emplace_back();
	gEnemyList.back().Initialize(position);
}
