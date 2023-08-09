#include "DemoScene.h"
#include <ClearDrawScreen.h>
#include <Sprite.h>
#include <SceneManager.h>
#include "AssimpLoader.h"
#include <TimeManager.h>
#include <MathF.h>
#include <ImguiManager.h>

void DemoScene::Initialize()
{
	sceneID = "Demo";

	mCamera->Initialize();

	mDebugCamera.Initialize();

	mSkydome.Initialize();
	mSkydome.SetModel(ModelManager::Get()->GetModel("skydome"));
	mSkydome.SetTexture(TextureManager::Get()->GetTexture("white"));
	mSkydome.scale = { 10,10,10 };
	mSkydome.color_ = { 1.f,1.f,1.f,1.0f };

	mCamera->mEye.y = 50;
	mCamera->mEye.z = -150;

	ModelManager::LoadModel("beetle","beetle");
	//ModelManager::LoadModel("firewisp","firewisp");
	ModelManager::LoadModel("boss","boss");
	
	mSlime.SetTexture(*TextureManager::GetTexture("slime"));
	mSlime.mPosition = { 0,0,0 };

	testplayer.Initialize();
	testplayer.SetModel(ModelManager::Get()->GetModel("firewisp"));
	testplayer.SetTexture(TextureManager::Get()->GetTexture("white"));

	LevelLoader::Get()->Load("Scene/default", "default",-1);
	LevelLoader::Get()->Load("Scene/woods", "woods",-1);

	currentLevel = "default";

	SetObject(*LevelLoader::Get()->GetData(currentLevel));
}

GUI gui("operater");

void DemoScene::Update()
{
	if (Input::Keyboard::TriggerKey(DIK_R))
	{
		mObj3ds.clear();
		std::string hoge = "Scene/";
		hoge += currentLevel;
		LevelLoader::Get()->Load(hoge, currentLevel,-1);
		SetObject(*LevelLoader::Get()->GetData(currentLevel));
	}

	mCamera->UpdatematView();

	mSkydome.Update(*mCamera);

	oldcurrentLevel = currentLevel;

	/*gui.Begin({ 100,100 }, { 300,300 });
	if (ImGui::Button("default"))
	{
		currentLevel = "default";
	}
	if (ImGui::Button("woods"))
	{
		currentLevel = "woods";
	}
	if (oldcurrentLevel != currentLevel)
	{
		mObj3ds.clear();
		std::string hoge = "Scene/";
		hoge += currentLevel;
		LevelLoader::Get()->Load(hoge, currentLevel);
		SetObject(*LevelLoader::Get()->GetData(currentLevel));
	}

	if (ImGui::Button("Change_Show_Object"))
	{
		showObject = !showObject;
	}
	if (ImGui::Button("Change_Show_Collider"))
	{
		showCollider = !showCollider;
	}
	if (ImGui::Button("Change_Show_SpawnPoint"))
	{
		showSpawn = !showSpawn;
	}
	if (ImGui::Button("Change_Show_EventTriger"))
	{
		showEvent = !showEvent;
	}
	ImGui::Text("currentLevel %s", currentLevel);
	ImGui::Text("showObject %d", showObject);
	ImGui::Text("showCollider %d", showCollider);
	ImGui::Text("showSpawn %d", showSpawn);
	ImGui::Text("showEvent %d", showEvent);

	gui.End();*/

	gui.Begin({ 100,100 }, { 200,100 });
	
	if (ImGui::Button("Phong"))
	{
		pipeline = "Phong";
	}
	if (ImGui::Button("SingleColor"))
	{
		pipeline = "SingleColor";
	}
	if (ImGui::Button("showSkydome"))
	{
		showSkydome = !showSkydome;
	}

	gui.End();

	if (oldcurrentLevel == currentLevel)
	{
		for (auto& obj : mObj3ds)
		{
			obj.Update(*mCamera);

			if (obj.MODEL->mSaveModelname != "BlankCube")
			{
				obj.mIsVisiable = showObject;
			}
			if (obj.MODEL->mSaveModelname == "BlankCube")
			{
				obj.mIsVisiable = showCollider;
			}
			if (obj.MODEL->mSaveModelname == "spawnpoint")
			{
				obj.mIsVisiable = showSpawn;
			}
			if (obj.MODEL->mSaveModelname == "eventtriger")
			{
				obj.mIsVisiable = showEvent;
			}
		}
	}

	mSlime.Update();

	mDebugCamera.Update();

	testplayer.Update(*mCamera);
}

void DemoScene::Draw()
{
	//BasicObjectPreDraw(PipelineManager::GetPipeLine("MultiRender"));
	BasicObjectPreDraw(PipelineManager::GetPipeLine(pipeline));

	if (showSkydome)
	{
		mSkydome.DrawMaterial();
	}

	for (auto& obj : mObj3ds)
	{
		if (obj.MODEL->mSaveModelname == "BlankCube")
		{
			obj.Draw();
		}
		else
		{
			obj.DrawMaterial();
		}
	}

	testplayer.DrawMaterial();

	//スプライトの前描画(共通コマンド)
	SpriteCommonBeginDraw();

	mSlime.Draw();
}

void DemoScene::End()
{
	mObj3ds.clear();
}

void DemoScene::SetObject(LevelData& data)
{
	/*auto itr = particles.begin();
	itr != particles.end(); itr++*/
	for (auto objectData = data.mObjects.begin(); objectData != data.mObjects.end(); objectData++)
	{
		//スポーンポイントを設定
		//指定された名前が入ってるなら対応したやつを入れる
		if (objectData->spawnpointName == "player")
		{
			testplayer.position = objectData->translation;
			testplayer.rotation = objectData->rotation;
			testplayer.scale = objectData->scaling;
		}
		else if (objectData->spawnpointName == "enemy")
		{
			//とりあえずキューブで配置
			mObj3ds.emplace_back();
			mObj3ds.back().Initialize();

			mObj3ds.back().SetModel(ModelManager::GetModel("spawnpoint"));
			//座標
			mObj3ds.back().position = objectData->translation;
			//回転角
			mObj3ds.back().rotation = {
					MathF::AngleConvRad(objectData->rotation.x),
					MathF::AngleConvRad(objectData->rotation.y),
					MathF::AngleConvRad(objectData->rotation.z)
			};
			//大きさ
			mObj3ds.back().scale = objectData->scaling;
		}
		else
		{
			//とりあえずキューブで配置
			//TODO:file_nameから逆引きできるようにしたい
			mObj3ds.emplace_back();
			mObj3ds.back().Initialize();
			//モデル設定
			//ファイルネームが設定されてるならそれで
			if (objectData->fileName != "")
			{
				//読み込みしてないなら読み込みも行う
				if (ModelManager::GetModel(objectData->fileName) == nullptr)
				{
					ModelManager::LoadModel(objectData->fileName, objectData->fileName);
				}
				mObj3ds.back().SetModel(ModelManager::GetModel(objectData->fileName));
			}
			//ないなら四角をデフォで設定
			else
			{
				mObj3ds.back().SetModel(ModelManager::GetModel("Cube"));
			}
			mObj3ds.back().SetTexture(TextureManager::Get()->GetTexture("white"));
			//座標
			mObj3ds.back().position = objectData->translation;
			//回転角
			mObj3ds.back().rotation = {
					MathF::AngleConvRad(objectData->rotation.x),
					MathF::AngleConvRad(objectData->rotation.y),
					MathF::AngleConvRad(objectData->rotation.z)
			};
			//大きさ
			mObj3ds.back().scale = objectData->scaling;
		
			//当たり判定を作成
			if (objectData->collider.have)
			{
				mObj3ds.emplace_back();
				mObj3ds.back().Initialize();

				mObj3ds.back().SetModel(ModelManager::GetModel("BlankCube"));
				mObj3ds.back().SetTexture(TextureManager::Get()->GetTexture("white"));

				mObj3ds.back().position = objectData->translation + objectData->collider.center;
				mObj3ds.back().scale = {
					objectData->scaling.x * objectData->collider.size.x,
					objectData->scaling.y * objectData->collider.size.y,
					objectData->scaling.z * objectData->collider.size.z
				};
				mObj3ds.back().rotation = {
					MathF::AngleConvRad(objectData->rotation.x),
					MathF::AngleConvRad(objectData->rotation.y),
					MathF::AngleConvRad(objectData->rotation.z)
				};
			}
		}
	}
}
