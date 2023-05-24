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
	skydome.SetModel(ModelManager::Get()->GetModel("skydome"));
	skydome.SetTexture(&TextureManager::Get()->white);
	skydome.scale = { 10,10,10 };
	skydome.color = { 0.05f,0.05f,0.05f,1.0f };
	skydome.model->material.ambient = { 0,0,0 };
	skydome.model->material.diffuse = { 0,0,0 };
	skydome.model->material.specular = { 0,0,0 };

	goalSprite.SetTexture(TextureManager::GetTexture("goal"));
	goalSprite.SetPos({ Util::window_width / 2, Util::window_height / 2 });
	SpriteSetSize(goalSprite, { 1280,720 });
	
	TitleSprite.SetTexture(TextureManager::GetTexture("Title"));
	TitleSprite.SetPos({ Util::window_width / 2,Util::window_height / 2 });
	SpriteSetSize(TitleSprite, { 1280,720 });

	startSprite.SetTexture(TextureManager::GetTexture("Start"));
	startSprite.SetPos({ Util::window_width / 2,Util::window_height / 2 + 100 });
	SpriteSetSize(startSprite, { 600,150 });
	
	spaceSprite.SetTexture(TextureManager::GetTexture("Space"));
	spaceSprite.SetPos({ Util::window_width / 2,Util::window_height / 2 + 200 });
	spaceSprite.SetColor({1,1,1,0});
	SpriteSetSize(spaceSprite, { 600,150 });

	redScreenSprite.SetTexture(TextureManager::GetTexture("redScreen"));
	redScreenSprite.SetPos({ Util::window_width / 2,Util::window_height / 2 });
	SpriteSetSize(redScreenSprite, { Util::window_width,Util::window_height });
	
	slimeSprite.SetTexture(TextureManager::GetTexture("slime"));
	slimeSprite.SetPos({ Util::window_width - 100,100 });

	gameOverSprite.SetTexture(TextureManager::GetTexture("GameOver"));
	gameOverSprite.SetPos({ Util::window_width / 2,Util::window_height / 2 });
	
	//ビュー変換行列(透視投影)を計算
	camera->Initialize();

	stage.SetStage1();

	push = SoundManager::GetInstance()->SoundLoadWave("Resources\\sound\\push.wav");
	goalSound = SoundManager::GetInstance()->SoundLoadWave("Resources\\sound\\goal.wav");
	deadEnemy = SoundManager::GetInstance()->SoundLoadWave("Resources\\sound\\deadenemy.wav");
	
	firewispsmooth.Initialize();
	firewispsmooth.position = { 60,-20,0 };
	firewispsmooth.scale = { 10,10,10 };
	firewispsmooth.model = ModelManager::Get()->GetModel("firewisp");
	firewispsmooth.texture = &TextureManager::Get()->white;
	
	firewispCheckTriangle.Initialize();
	firewispCheckTriangle.position = { 30,-20,0 };
	firewispCheckTriangle.scale = { 10,10,10 };
	firewispCheckTriangle.model = ModelManager::Get()->GetModel("sphere");
	firewispCheckTriangle.texture = &TextureManager::Get()->white;
	
	board.Initialize();
	board.rotation = { 0,0,MathF::DegConvRad(270) };
	board.scale = { 10,10,10 };
	board.model = ModelManager::Get()->ModelManager::Get()->GetModel("board");
	board.texture = &TextureManager::Get()->white;
	
	triangle.Initialize();
	triangle.position = { 40,-20,0 };
	triangle.scale = { 10,10,10 };
	triangle.model = ModelManager::Get()->GetModel("triangle");
	triangle.texture = &TextureManager::Get()->white;

	float a = -2.0f;
	float b = 2.0f;

	int r = 0;

	for (int i = 0; i < max; i++)
	{
		cube[i].Initialize();
		cube[i].SetModel(ModelManager::Get()->GetModel("Cube"));
		cube[i].SetTexture(&TextureManager::Get()->white);

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

	lightGroup = LightGroup::Create();
	//lightGroup->SetDirLightActive(0, false);
	//lightGroup->SetDirLightActive(1, false);
	//lightGroup->SetDirLightActive(2, false);

	lightGroup->SetPointLightActive(0, true);
	lightGroup->SetPointLightPos(0, { 0.5f,1.0f,0.0f });
	LightGroup::SetLight(lightGroup);

	lightObj.Initialize();
	lightObj.position = { 0.5f,0,0 };
	lightObj.scale = { 3,3,3 };
	lightObj.model = ModelManager::Get()->GetModel("Cube");
	lightObj.texture = &TextureManager::Get()->white;
}

//GUI gui2("Light");

void Game::Update()
{

//#pragma region Imguiの処理
//	ImGui::Text("FPS %f", TimeManager::fps);
//	ImGui::Text("MAXFPS %f", TimeManager::fixFPS);
//	
//	if (ImGui::Button("30FPS"))
//	{
//		TimeManager::fps = 0;
//		TimeManager::fixFPS = 30;
//	}
//	ImGui::SameLine();
//	if (ImGui::Button("60FPS"))
//	{
//		TimeManager::fps = 0;
//		TimeManager::fixFPS = 60;
//	}
//	ImGui::SameLine();
//	if (ImGui::Button("144FPS"))
//	{
//		TimeManager::fps = 0;
//		TimeManager::fixFPS = 144;
//	}
//
//	ImGui::Text("CheckConnectPad %d", input->CheckConnectPad());
//
//	if (ImGui::Button("LAMBERT"))
//	{
//		object3dPipelineSet = CreateLambertPipeline();
//	}
//	ImGui::SameLine();
//	if (ImGui::Button("PHONG"))
//	{
//		object3dPipelineSet = CreateObject3DPipeline();
//	}
//
//	static float lightColor[3] = { 1.0f,0,0};
//	static float lightColor2[3] = { 0,1.0f,0 };
//	static float lightColor3[3] = { 0,0,1.0f };
//
//	static float lightDir[3] = {0,-1.0f,0};
//	static float lightDir2[3] = { 0.5f,0.1f,0.2f };
//	static float lightDir3[3] = { -0.5f,0.1f,-0.2f };
//
//	static float lightPointPos[3] = { 0.5f,1,0 };
//	static float lightPointColor[3] = { 1,1,1 };
//	
//	gui2.Begin({ 10,100 }, { 500,300 });
//	ImGui::ColorEdit3("light1RGB", lightColor);
//	ImGui::ColorEdit3("light2RGB", lightColor2);
//	ImGui::ColorEdit3("light3RGB", lightColor3);
//	ImGui::SliderFloat3("light1Dir", lightDir,-10,10);
//	ImGui::SliderFloat3("light2Dir", lightDir2, -10, 10);
//	ImGui::SliderFloat3("light3Dir", lightDir3, -10, 10);
//	ImGui::Text("%f %f %f", 
//		firewispsmooth.position.x,
//		firewispsmooth.position.y,
//		firewispsmooth.position.z
//		);
//	
//	ImGui::ColorEdit3("lightPointRGB", lightPointColor);
//	ImGui::SliderFloat3("lightPointPos", lightPointPos, -100, 100);
//
//
//	if (ImGui::Button("lightReset"))
//	{
//		lightColor[0] = 1.0f;
//		lightColor[1] = 0;
//		lightColor[2] = 0;
//
//		lightColor2[0] = 0;
//		lightColor2[1] = 1.0f;
//		lightColor2[2] = 0;
//		
//		lightColor3[0] = 0.0f;
//		lightColor3[1] = 0.0f;
//		lightColor3[2] = 1.0f;
//
//		lightDir[0] = 0;
//		lightDir[1] = -1.0f;
//		lightDir[2] = 0;
//
//		lightDir2[0] = 0.5f;
//		lightDir2[1] = 0.1f;
//		lightDir2[2] = 0.2f;
//
//		lightDir3[0] = -0.5f;
//		lightDir3[1] = 0.1f;
//		lightDir3[2] = -0.2f;
//	}
//	ImGui::SameLine();
//
//	static bool hoge2 = true;
//	static bool hoge3 = true;
//
//	if (ImGui::Button("DirActive"))
//	{
//		hoge2 = !hoge2;
//
//		lightGroup->SetDirLightActive(0, hoge2);
//		lightGroup->SetDirLightActive(1, hoge2);
//		lightGroup->SetDirLightActive(2, hoge2);
//	}
//	ImGui::SameLine();
//	if (ImGui::Button("PointActive"))
//	{
//		hoge3 = !hoge3;
//
//		lightGroup->SetPointLightActive(0, hoge3);
//	}
//
//	gui2.End();
//
//	lightGroup->SetDirLightColor(0, { lightColor[0], lightColor[1] , lightColor[2]});
//	lightGroup->SetDirLightColor(1, { lightColor2[0], lightColor2[1] , lightColor2[2] });
//	lightGroup->SetDirLightColor(2, { lightColor3[0], lightColor3[1] , lightColor3[2] });
//
//	lightGroup->SetDirLightDir(0, { lightDir[0],lightDir[1] ,lightDir[2]});
//	lightGroup->SetDirLightDir(1, { lightDir2[0],lightDir2[1] ,lightDir2[2] });
//	lightGroup->SetDirLightDir(2, { lightDir3[0],lightDir3[1] ,lightDir3[2] });
//	
//	lightGroup->SetPointLightPos(0, { lightPointPos[0],lightPointPos[1] ,lightPointPos[2] });
//	lightGroup->SetPointLightColor(0, { lightPointColor[0],lightPointColor[1] ,lightPointColor[2] });
//
//	//lightObj.position = lightGroup->pointLights->lightPos;
//
//#pragma endregion

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

	if (redScreenSprite.color.f4.w > 0)
	{
		redScreenSprite.color.f4.w -= TimeManager::deltaTime;
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

	//当たり判定実装
	//球と無限平面の当たり判定
	Sphere sphereCol;
	sphereCol.center = firewispsmooth.position;
	sphereCol.radius = firewispsmooth.scale.x * 2;

	Plane planeCol;
	planeCol.normal = { 0,1,0 };
	planeCol.distance = board.position.y - 1.0f;

	static float timer = 0.0f;
	static float timechange = 1.0f;
	if (timer > 1.0f)
	{
		timechange = -1.0f;
	}
	if (timer < -1.0f)
	{
		timechange = 1.0f;
	}
	timer += timechange * TimeManager::deltaTime;

	if (input->PushKey(DIK_7))sphereVal.Start();
	sphereVal.Update();
	firewispsmooth.position = Vector3::easeInQuad({50,0,30},{-50,0,30}, sphereVal.GetTimeRate());
	firewispCheckTriangle.position = Vector3::easeInOutQuad({ 50,-10,30 }, { -50,-10,30 }, sphereVal.GetTimeRate());
	lightObj.position = Vector3::easeOutQuad({ 50,10,30 }, { -50,10,30 }, sphereVal.GetTimeRate());

	if (SpherePlaneCollision(sphereCol, planeCol))
	{
		firewispsmooth.color = { 1,0,0,1 };
	}
	else
	{
		firewispsmooth.color = { 1,1,1,1 };
	}

	firewispsmooth.rotation.y += TimeManager::deltaTime;
	firewispsmooth.Update(*camera);

	board.position = { 60,-30.0f,0 };

	board.Update(*camera);

	////球と三角形の当たり判定(上手にできてない)
	//ImGui::SliderFloat("triangle.x", &triangle.position.x, -100, 100);
	//ImGui::SliderFloat("triangle.y", &triangle.position.y, -100, 100);
	//ImGui::SliderFloat("triangle.z", &triangle.position.z, -100, 100);

	Triangle triangleCol;
	triangleCol.pos0 = {
		triangle.position.x + (triangle.model->mesh.vertices[0].pos.x * triangle.scale.x / 2),
		triangle.position.y + (triangle.model->mesh.vertices[0].pos.y * triangle.scale.y / 2),
		triangle.position.z + (triangle.model->mesh.vertices[0].pos.z * triangle.scale.z / 2)
	};

	triangleCol.pos1 = {
		triangle.position.x + (triangle.model->mesh.vertices[1].pos.x * triangle.scale.x / 2),
		triangle.position.y + (triangle.model->mesh.vertices[1].pos.y * triangle.scale.y / 2),
		triangle.position.z + (triangle.model->mesh.vertices[1].pos.z * triangle.scale.z / 2)
	};

	triangleCol.pos2 = {
		triangle.position.x + (triangle.model->mesh.vertices[2].pos.x * triangle.scale.x / 2),
		triangle.position.y + (triangle.model->mesh.vertices[2].pos.y * triangle.scale.y / 2),
		triangle.position.z + (triangle.model->mesh.vertices[2].pos.z * triangle.scale.z / 2)
	};

	triangleCol.normal = {
		triangle.model->mesh.vertices[0].normal.x,
		triangle.model->mesh.vertices[0].normal.y,
		triangle.model->mesh.vertices[0].normal.z
	};

	Sphere sphereColCheckTri;
	sphereColCheckTri.center = firewispCheckTriangle.position;
	sphereColCheckTri.radius = firewispCheckTriangle.scale.x;

	
	if (CheckSphere2Triangle(sphereColCheckTri, triangleCol))
	{
		firewispCheckTriangle.color = { 1,0,0,1 };
	}
	else
	{
		firewispCheckTriangle.color = { 1,1,1,1 };
	}

	triangle.Update(*camera);

	firewispCheckTriangle.Update(*camera);

	redScreenSprite.Update(); //SpriteUpdate(redScreenSprite, SpriteCommon::spriteCommon);
	slimeSprite.Update(); //SpriteUpdate(slimeSprite, SpriteCommon::spriteCommon);
	ParticleManager::GetInstance()->Update();
	
	lightObj.Update(*camera);

	lightGroup->Update();
}

void Game::Draw()
{
	BasicObjectPreDraw(object3dPipelineSet);

	//描画処理
	stage.Draw();

	lightObj.Draw();

	player.Draw();

	firewispsmooth.Draw();
	board.Draw();

	firewispCheckTriangle.Draw();
	triangle.Draw();

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

	//GeometryObjectPreDraw(geometryObjectPipelineSet);

	//スプライトの前描画(共通コマンド)
	SpriteCommonBeginDraw(SpriteCommon::spriteCommon);

	redScreenSprite.Draw();
	slimeSprite.Draw();
}

void Game::End()
{
	ParticleManager::GetInstance()->AllDelete();

	player.End();
	SoundManager::GetInstance()->SoundUnload(&push);
	SoundManager::GetInstance()->SoundUnload(&goalSound);

	delete lightGroup;
}

int upCam = 0;
float mag = 20;

void Game::CameraUpdate()
{
	/*if (input->TriggerKey(DIK_1))
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
	}*/

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
		redScreenSprite.color.f4.w = 1.0f;
		for (int i = 0; i < 10; i++)
		{
			ParticleManager::GetInstance()->CreateCubeParticle(player.position, { 3,3,3 }, 20.0f, { 1,1,1,1 });
		}
	}
}
