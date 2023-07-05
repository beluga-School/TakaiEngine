#include "Stage.h"
#include "Model.h"
#include "MathF.h"
#include "EnemyManager.h"
#include "Player.h"
#include "WarpBlock.h"
#include "GoalBlock.h"
#include "ClearDrawScreen.h"

void Stage::ChangeLevel(LevelData& data)
{
	//ハンドルをステージに保存
	currentHandle = data.mHandle;

	//入ってたものを削除
	mObj3ds.clear();
	mColCubes.clear();
	mEventObjects.clear();
	mColObj3ds.clear();

	for (auto objectData = data.mObjects.begin(); objectData != data.mObjects.end(); objectData++)
	{
		//プレイヤーの配置なら
		if (objectData->setObjectName == "player")
		{
			Player::Get()->Reset();
			Player::Get()->preMove = objectData->translation;
			Player::Get()->position = objectData->translation;
			Player::Get()->rotation = objectData->rotation;
			Player::Get()->scale = objectData->scaling;

			continue;
		}
		
		//エネミーの配置なら
		if (objectData->setObjectName == "enemy")
		{
			EnemyManager::Get()->Load(*objectData);
			continue;
		}

		//イベントオブジェクトなら設置
		if (objectData->eventtrigerName != "")
		{
			//中でさらに分類わけして配置している
			//EventBlock を基底クラスに、HitEffectの中身を変えたクラスで実装している
			EvenyObjectSet(*objectData);
			continue;
		}

		if (objectData->spawnpointName == "enemy")
		{
			//とりあえずキューブで配置
			mObj3ds.emplace_back();
			mObj3ds.back().Initialize();

			mObj3ds.back().SetModel(ModelManager::GetModel("spawnpoint"));

			LevelDataExchanger::SetObjectData(mObj3ds.back(), *objectData);
			
			continue;
		}

		//---ここより前でcontinue忘れると、モデルを読み込んじゃってバグる可能性大

		//なにも無かったら
		{
			//そのままモデルの配置
			NormalObjectSet(*objectData);

			//当たり判定を作成
			if (objectData->collider.have)
			{
				CollisionSet(*objectData);
			}

			continue;
		}
	}
}

void Stage::Update()
{
	for (auto &obj: mObj3ds)
	{
		obj.Update(*Camera::sCamera);
	}

	for (auto& obj : mEventObjects)
	{
		obj->Update();
	}

	for (auto& obj : mColObj3ds)
	{
		obj.Update(*Camera::sCamera);
	}

	goalSystem.Update();
}

void Stage::Draw()
{
	//物によってマテリアル描画とテクスチャ描画が混在してるのに
	//分ける方法を作ってないので作る
	DrawCollider();

	DrawModel();
}

void Stage::DrawSprite()
{
	goalSystem.Draw();
}

std::string Stage::GetNowStageHandle()
{
	return currentHandle;
}

void Stage::NormalObjectSet(const LevelData::ObjectData& data)
{
	//コリジョン目的で配置したならオブジェクト配置を行わない
	if (data.fileName == "collision")
	{
		return;
	}

	//とりあえずキューブで配置
	mObj3ds.emplace_back();
	mObj3ds.back().Initialize();

	//アウトライン設定
	mObj3ds.back().SetOutLineState({ 1,0,0 }, 0.05f);

	//モデル設定
	//ファイルネームが設定されてるならそれで
	if (data.fileName != "")
	{
		//読み込みしてないなら読み込みも行う
		if (ModelManager::GetModel(data.fileName) == nullptr)
		{
			ModelManager::LoadModel(data.fileName, data.fileName,true);
		}
		mObj3ds.back().SetModel(ModelManager::GetModel(data.fileName));
	}
	//ないなら四角をデフォで設定
	else
	{
		mObj3ds.back().SetModel(ModelManager::GetModel("Cube"));
	}
	//バグらないように白テクスチャを入れる
	mObj3ds.back().SetTexture(TextureManager::Get()->GetTexture("white"));
	
	//オブジェクトの配置
	LevelDataExchanger::SetObjectData(mObj3ds.back(), data);
}

void Stage::CollisionSet(const LevelData::ObjectData& data)
{
	//当たり判定を表示するオブジェクト
	mColObj3ds.emplace_back();
	mColObj3ds.back().Initialize();

	mColObj3ds.back().SetModel(ModelManager::GetModel("BlankCube"));
	mColObj3ds.back().SetTexture(TextureManager::Get()->GetTexture("white"));

	mColObj3ds.back().position = data.translation + data.collider.center;
	mColObj3ds.back().scale = {
		data.scaling.x * data.collider.size.x,
		data.scaling.y * data.collider.size.y,
		data.scaling.z * data.collider.size.z
	};
	mColObj3ds.back().rotation = {
		MathF::AngleConvRad(data.rotation.x),
		MathF::AngleConvRad(data.rotation.y),
		MathF::AngleConvRad(data.rotation.z)
	};

	//当たり判定自体の情報を作成
	mColCubes.emplace_back();
	mColCubes.back().position = data.translation + data.collider.center;
	mColCubes.back().scale = {
		data.scaling.x * data.collider.size.x,
		data.scaling.y * data.collider.size.y,
		data.scaling.z * data.collider.size.z
	};
}

void Stage::EvenyObjectSet(const LevelData::ObjectData& data)
{
	//stage の文字列が含まれてるなら
	if (data.eventtrigerName.find("stage") != std::string::npos)
	{
		mEventObjects.emplace_back();
		mEventObjects.back() = std::make_unique<WarpBlock>();
		mEventObjects.back()->Initialize();

		mEventObjects.back()->SetOutLineState({ 1,0,0 }, 0.05f);

		mEventObjects.back()->trigerName = data.eventtrigerName;
		//バグらないように白テクスチャを入れる
		mEventObjects.back()->SetTexture(TextureManager::Get()->GetTexture("white"));

		//オブジェクトの配置
		LevelDataExchanger::SetObjectData(*mEventObjects.back(), data);
	}
	//goal の文字列が含まれてるなら
	if (data.eventtrigerName.find("goal") != std::string::npos)
	{
		mEventObjects.emplace_back();
		mEventObjects.back() = std::make_unique<GoalBlock>();
		mEventObjects.back()->Initialize();
		mEventObjects.back()->trigerName = data.eventtrigerName;
		//バグらないように白テクスチャを入れる
		mEventObjects.back()->SetTexture(TextureManager::Get()->GetTexture("white"));

		//オブジェクトの配置
		LevelDataExchanger::SetObjectData(*mEventObjects.back(), data);
	}
}

void Stage::DrawModel()
{
	if (mShowModel == false) return;
	for (auto& obj : mObj3ds)
	{
		BasicObjectPreDraw(PipelineManager::GetPipeLine("OutLine"), false);
		obj.DrawOutLine();
		BasicObjectPreDraw(PipelineManager::GetPipeLine("GroundToon"));
		obj.DrawMaterial();
	}
	for (auto& obj : mEventObjects)
	{
		BasicObjectPreDraw(PipelineManager::GetPipeLine("OutLine"),false);
		obj->DrawOutLine();
		BasicObjectPreDraw(PipelineManager::GetPipeLine("GroundToon"));
		obj->Draw();
	}
}

void Stage::DrawCollider()
{
	if (mShowCollider == false) return;
	for (auto& obj : mColObj3ds)
	{
		obj.Draw();
	}
}
