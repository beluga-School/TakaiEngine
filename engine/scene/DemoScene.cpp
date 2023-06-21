#include "DemoScene.h"
#include <ClearDrawScreen.h>
#include <Sprite.h>
#include <SceneManager.h>
#include "GameScene.h"
#include "AssimpLoader.h"
#include <TimeManager.h>

void DemoScene::Initialize()
{
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
	ModelManager::LoadModel("firewisp","firewisp");
	ModelManager::LoadModel("boss","boss");
	ModelManager::LoadModel("BlankCube","BlankCube");
	
	mSlime.SetTexture(*TextureManager::GetTexture("slime"));
	mSlime.mPosition = { 0,0,0 };

	testplayer.Initialize();
	testplayer.SetModel(ModelManager::Get()->GetModel("firewisp"));
	testplayer.SetTexture(TextureManager::Get()->GetTexture("white"));

	LevelLoader::Get()->Load("Scene/worldTest_Children", "children");
	LevelLoader::Get()->Load("Scene/playerTest", "pTest");
	SetObject(*LevelLoader::Get()->GetData("children"));
}

void DemoScene::Update()
{
	if (Input::Keyboard::TriggerKey(DIK_O))
	{
		mSceneManager->ChangeScene<Game>();
		//sceneManager->ChangeScene("GAMEPLAY");
	}

	if (Input::Keyboard::TriggerKey(DIK_R))
	{
		mObj3ds.clear();

		SetObject(*LevelLoader::Get()->GetData("pTest"));
	}

	mCamera->UpdatematView();

	mSkydome.Update(*mCamera);
	for (auto& obj : mObj3ds)
	{
		obj.Update(*mCamera);
	}

	mSlime.Update();

	mDebugCamera.Update();

	testplayer.Update(*mCamera);
}

void DemoScene::Draw()
{
	BasicObjectPreDraw(PipelineManager::GetPipeLine("Object3D"));

	mSkydome.DrawMaterial();

	for (auto& obj : mObj3ds)
	{
		obj.Draw();
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
			mObj3ds.back().rotation = objectData->rotation;
			//大きさ
			mObj3ds.back().scale = objectData->scaling;
		}
		
		//当たり判定を作成
		if (objectData->collider.have)
		{
			mObj3ds.emplace_back();
			mObj3ds.back().Initialize();

			mObj3ds.back().SetModel(ModelManager::GetModel("BlankCube"));
			mObj3ds.back().SetTexture(TextureManager::Get()->GetTexture("white"));

			mObj3ds.back().position = objectData->translation;
			mObj3ds.back().scale = objectData->collider.size;
			mObj3ds.back().rotation = { 0,0,0 };
		}
	}
}
