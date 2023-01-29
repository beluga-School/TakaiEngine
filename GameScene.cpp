#include "GameScene.h"
#include "TimeManager.h"
#include "Collision.h"
#include "ImguiManager.h"
#include "MathF.h"
#include "ObjParticle.h"
#include "Quaternion.h"

void Game::Initialize()
{
	//3dオブジェクト用のパイプライン生成
	object3dPipelineSet = CreateObject3DPipeline();

	//ジオメトリオブジェクト用パイプライン生成
	geometryObjectPipelineSet = CreateGeometryPipeline();

	//スプライト

	skydome.Initialize();
	skydome.SetModel(&ModelManager::GetInstance()->skyDomeM);
	skydome.SetTexture(&TextureManager::GetInstance()->white);
	skydome.scale = { 10,10,10 };
	skydome.color = { 0.05f,0.05f,0.05f,1.0f };
	skydome.model->material.ambient = { 0,0,0 };
	skydome.model->material.diffuse = { 0,0,0 };
	skydome.model->material.specular = { 0,0,0 };

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

	SpriteCreate(&redScreenSprite, &TextureManager::GetInstance()->redScreen, { 0.5f,0.5f });
	SpriteInit(redScreenSprite, SpriteCommon::spriteCommon,
		{ Util::window_width / 2,Util::window_height / 2 }, 0);
	SpriteSetSize(redScreenSprite, { Util::window_width,Util::window_height });
	
	SpriteCreate(&setumeiSprite, &TextureManager::GetInstance()->setumei, { 0.5f,0.5f });
	SpriteInit(setumeiSprite, SpriteCommon::spriteCommon,
		{200,170 }, 0);
	SpriteSetSize(setumeiSprite, { 300 * 1.25f,250 * 1.25f });
	
	SpriteCreate(&setumei2Sprite, &TextureManager::GetInstance()->setumei2, { 0.5f,0.5f });
	SpriteInit(setumei2Sprite, SpriteCommon::spriteCommon,
		{Util::window_width / 2 + 420,170 }, 0);
	SpriteSetSize(setumei2Sprite, { 300 * 1.25f,250 * 1.25f });
	
	SpriteCreate(&gameOverSprite, &TextureManager::GetInstance()->gameOver, { 0.5f,0.5f });
	SpriteInit(gameOverSprite, SpriteCommon::spriteCommon,
		{ Util::window_width / 2,Util::window_height / 2 }, 0);

	//ビュー変換行列(透視投影)を計算
	camera->Initialize();

	redScreenSprite.color.w = 0.0f;

	stage.SetStage1();

	push = SoundManager::GetInstance()->SoundLoadWave("Resources\\sound\\push.wav");
	goalSound = SoundManager::GetInstance()->SoundLoadWave("Resources\\sound\\goal.wav");
	deadEnemy = SoundManager::GetInstance()->SoundLoadWave("Resources\\sound\\deadenemy.wav");
	
	firewispnorm.Initialize();
	firewispnorm.position = {0,-20,0};
	firewispnorm.scale = { 10,10,10 };
	firewispnorm.model = &ModelManager::GetInstance()->firewispM;
	firewispnorm.texture = &TextureManager::GetInstance()->white;
	
	firewispsmooth.Initialize();
	firewispsmooth.position = { 20,-20,0 };
	firewispsmooth.scale = { 10,10,10 };
	firewispsmooth.model = &ModelManager::GetInstance()->firewispSmoothingM;
	firewispsmooth.texture = &TextureManager::GetInstance()->white;

	float a = -2.0f;
	float b = 2.0f;

	int r = 0;

	for (int i = 0; i < max; i++)
	{
		cube[i].Initialize();
		cube[i].SetModel(&ModelManager::GetInstance()->cubeM);
		cube[i].SetTexture(&TextureManager::GetInstance()->white);

		cube[i].scale = { MathF::GetRand(3,7),MathF::GetRand(3,7),MathF::GetRand(3,7) };

		r = MathF::GetRandInt(0,4);

		switch (r)
		{
		case 0:
			cube[i].color = { 0.0f, 220.0f / 255.0f, 1.0f, 1 };
			break;
		case 1:
			cube[i].color = { 220.0f / 255.0f, 1.0f, 0.0f, 1 };
			break;
		case 2:
			cube[i].color = { 1.0f,0.0f , 220.0f / 255.0f, 1 };
			break;
		}
	}

	for (int i = 0; i < 100; i++)
	{
		cube[i].position.x = MathF::GetRand(150.0f, 400.0f);
		cube[i].position.y = MathF::GetRand(-500.0f, 800.0f);
		cube[i].position.z = MathF::GetRand(-700.0f, 200.0f);

		cuberotaVec[i] = { MathF::GetRand(a,b),MathF::GetRand(a,b),MathF::GetRand(a,b) };
	}

	for (int i = 100; i < 200; i++)
	{
		cube[i].position.x = MathF::GetRand(-400.0f, -150.0f);
		cube[i].position.y = MathF::GetRand(-500.0f, 800.0f);
		cube[i].position.z = MathF::GetRand(-700.0f, 200.0f);

		cuberotaVec[i] = { MathF::GetRand(a,b),MathF::GetRand(a,b),MathF::GetRand(a,b) };
	}

	for (int i = 200; i < 300; i++)
	{
		cube[i].position.x = MathF::GetRand(-150.0f, 150.0f);
		cube[i].position.y = MathF::GetRand(-500.0f, -200.0f);
		cube[i].position.z = MathF::GetRand(-700.0f, 200.0f);

		cuberotaVec[i] = { MathF::GetRand(a,b),MathF::GetRand(a,b),MathF::GetRand(a,b) };
	}

	for (int i = 300; i < 400; i++)
	{
		cube[i].position.x = MathF::GetRand(-150.0f, 150.0f);
		cube[i].position.y = MathF::GetRand(500.0f, 700.0f);
		cube[i].position.z = MathF::GetRand(-700.0f, 200.0f);

		cuberotaVec[i] = { MathF::GetRand(a,b),MathF::GetRand(a,b),MathF::GetRand(a,b) };
	}

	for (int i = 400; i < 450; i++)
	{
		cube[i].position.x = MathF::GetRand(-150.0f, 150.0f);
		cube[i].position.y = MathF::GetRand(-500.0f, 700.0f);
		cube[i].position.z = MathF::GetRand(200.0f, 300.0f);

		cuberotaVec[i] = { MathF::GetRand(a,b),MathF::GetRand(a,b),MathF::GetRand(a,b) };
	}

	for (int i = 450; i < 500; i++)
	{
		cube[i].position.x = MathF::GetRand(-150.0f, 150.0f);
		cube[i].position.y = MathF::GetRand(-500.0f, 700.0f);
		cube[i].position.z = MathF::GetRand(-700.0f, -650.0f);

		cuberotaVec[i] = { MathF::GetRand(3.0f,5.0f),MathF::GetRand(3.0f,5.0f),MathF::GetRand(3.0f,5.0f) };
	}

	player.Initialize();

	Reset();

	light = Light::Create();
	light->SetLightColor({ 1,1,1 });
	Obj3d::SetLight(light);

	lightObj.Initialize();
	lightObj.scale = { 3,3,3 };
	lightObj.model = &ModelManager::GetInstance()->cubeM;
	lightObj.texture = &TextureManager::GetInstance()->white;
}

void Game::Update()
{
	setumeiEndTime += TimeManager::deltaTime;


	if (ImGui::Button("LAMBERT"))
	{
		Obj3d::mode = PipeLineMode::LAMBERT;
	}
	if (ImGui::Button("PHONG"))
	{
		Obj3d::mode = PipeLineMode::PHONG;
	}

	ImGui::SliderFloat("lightX", &lightObj.position.x, -10, 10);
	ImGui::SliderFloat("lightY", &lightObj.position.y, -10, 10);
	ImGui::SliderFloat("lightZ", &lightObj.position.z, -10, 10);

	light->SetLightDirection(lightObj.position);

	lightObj.Update(*camera);

	switch (Obj3d::mode)
	{
	case PipeLineMode::LAMBERT:
		object3dPipelineSet = CreateLambertPipeline();
		break;
	case PipeLineMode::PHONG:
		object3dPipelineSet = CreateObject3DPipeline();
		break;
	}

	CameraUpdate();
	//オブジェクトの更新
	skydome.Update(*camera);
	for (int i = 0; i < max; i++)
	{
		cube[i].rotation += cuberotaVec[i] * TimeManager::deltaTime;
		cube[i].Update(*camera);
	}

	stage.Update();

	if (player.position.y <= -200)
	{
		DamageEffect();
	}
	player.Update(stage);

	if (redScreenSprite.color.w > 0)
	{
		redScreenSprite.color.w -= TimeManager::deltaTime;
	}

	gEnemyList.remove_if([](GroundEnemy& enemy) {
		return enemy.isDead;
		});
	for (GroundEnemy& gEnemy : gEnemyList)
	{
		gEnemy.Update(stage, player);
		if (CubeCollision(player.cubeCol, gEnemy.cubeCol))
		{
			gEnemy.isDead = true;
			SoundManager::GetInstance()->SoundPlayWave(deadEnemy);
			for (int i = 0; i < 10; i++)
			{
				ParticleManager::GetInstance()->CreateCubeParticle(gEnemy.position, { 3,3,3 }, 20.0f, { 1.0f,1.0f,1.0f,1.0f });
			}

			DamageEffect();
			player.HitEffect();
		}
	}

	airEnemyList.remove_if([](AirEnemy& enemy) {
		return enemy.isDead;
		});
	for (AirEnemy& airEnemy : airEnemyList)
	{
		airEnemy.Update(stage, player);
		for (Bullet& bullet : airEnemy.bullets)
		{
			bullet.Update();
			if (CubeCollision(bullet.cubeCol, player.cubeCol))
			{
				bullet.isDead = true;
				DamageEffect();
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
			SoundManager::GetInstance()->SoundPlayWave(deadEnemy);
			for (int i = 0; i < 10; i++)
			{
				ParticleManager::GetInstance()->CreateCubeParticle(airEnemy.position, { 3,3,3 }, 20.0f, { 1.0f,1.0f,1.0f,1.0f });
			}
		}
	}

	if (setumeiEndTime > 20.0f)
	{
		if (setumeiSprite.color.w > 0)setumeiSprite.color.w -= TimeManager::deltaTime;
		if (setumei2Sprite.color.w > 0)setumei2Sprite.color.w -= TimeManager::deltaTime;
	}

	firewispnorm.rotation.y += TimeManager::deltaTime;
	firewispnorm.Update(*camera);
	
	firewispsmooth.rotation.y += TimeManager::deltaTime;
	firewispsmooth.Update(*camera);

	SpriteUpdate(redScreenSprite, SpriteCommon::spriteCommon);
	SpriteUpdate(setumeiSprite, SpriteCommon::spriteCommon);
	SpriteUpdate(setumei2Sprite, SpriteCommon::spriteCommon);

	ParticleManager::GetInstance()->Update();
}

void Game::Draw()
{
	BasicObjectPreDraw(object3dPipelineSet);

	//描画処理
	stage.Draw();

	lightObj.Draw();

	player.Draw();
	firewispnorm.DrawMaterial();
	firewispsmooth.DrawMaterial();

	for (GroundEnemy& gEnemy : gEnemyList)
	{
		gEnemy.DrawMaterial();
	}

	for (AirEnemy& airEnemy : airEnemyList)
	{
		airEnemy.Draw();
	}

	skydome.Draw();

	for (int i = 0; i < max; i++)
	{
		cube[i].Draw();
	}

	ParticleManager::GetInstance()->Draw();

	GeometryObjectPreDraw(geometryObjectPipelineSet);

	//スプライトの前描画(共通コマンド)
	SpriteCommonBeginDraw(SpriteCommon::spriteCommon);

	SpriteDraw(redScreenSprite);
}

void Game::End()
{
	ParticleManager::GetInstance()->AllDelete();

	player.End();
	SoundManager::GetInstance()->SoundUnload(&push);
	SoundManager::GetInstance()->SoundUnload(&goalSound);

	delete light;
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

		break;
	case 1:
		camera->eye = player.position;
		camera->eye.z += 1;
		camera->eye.y += 35;
		camera->target = player.position;
		break;
	case 2:
		camera->eye = player.position;
		camera->eye.z += 10;
		camera->target = player.position;
		break;
	}

	camera->UpdatematView();
}

//void Game::SceneChange(Scene scene)
//{
//	nextScene = scene;
//}
//
//void Game::SceneChangeUpdate()
//{
//	if (scene != nextScene)
//	{
//		sceneChangeBlockOut.color.w += TimeManager::deltaTime;
//		if (sceneChangeBlockOut.color.w >= 1)
//		{
//			sceneChangeBlockOut.color.w = 1;
// 			scene = nextScene;
//		}
//	}
//	else
//	{
//		sceneChangeBlockOut.color.w -= TimeManager::deltaTime;
//		if (sceneChangeBlockOut.color.w <= 0)
//		{
//			sceneChangeBlockOut.color.w = 0;
//		}
//	}
//}

void Game::SetAirEnemy(Vector3 position,bool hanten)
{
	airEnemyList.emplace_back();
	airEnemyList.back().Initialize(position);
	if (hanten)
	{
		airEnemyList.back().rotation.y = 3.14f / 2;
	}
}

void Game::SetGroundEnemy(Vector3 position, bool hanten)
{
	gEnemyList.emplace_back();
	gEnemyList.back().Initialize(position);
	if (hanten)
	{
		gEnemyList.back().rotation.y = 3.14f / 2;
	}
}

void Game::Reset()
{
	goalSoundFlag = false;
	setumeiEndTime = 0;
	setumeiSprite.color.w = 1;
	setumei2Sprite.color.w = 1;

	player.spawnPos = { 0,0,0 };
	player.hp = 10;
	player.Respawn();

	SetAirEnemy({ 0,30,-190 });
	SetAirEnemy({ 0,30,-210 });
	SetAirEnemy({ 0,30,-230 });

	SetAirEnemy({ 80,110,-440 });
	SetAirEnemy({ 0,140,-380 });

	SetAirEnemy({ 0,260,-480 },1);
	SetAirEnemy({ 50,260,-480 },1);
	SetAirEnemy({ -50,260,-480 },1);

	SetGroundEnemy({ 0,30,-190 });

	SetGroundEnemy({ 20,240,-240 },1);
	SetGroundEnemy({ -20,240,-240 },1);

	SetAirEnemy({ 0,300,50 },1);
	SetAirEnemy({ 0,330,70 },1);
	SetAirEnemy({ 0,360,90 },1);
}

void Game::DamageEffect()
{
	if (player.mutekiTimer <= 0 && player.isDash == false)
	{
		redScreenSprite.color.w = 1.0f;
		for (int i = 0; i < 10; i++)
		{
			ParticleManager::GetInstance()->CreateCubeParticle(player.position, { 3,3,3 }, 20.0f, { 1,1,1,1 });
		}
	}
}
