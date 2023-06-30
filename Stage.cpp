#include "Stage.h"
#include "Model.h"
#include "MathF.h"
#include "EnemyManager.h"

void Stage::ChangeLevel(LevelData& data)
{
	//入ってたものを削除
	mObj3ds.clear();
	mColCubes.clear();
	mEventObjects.clear();

	for (auto objectData = data.mObjects.begin(); objectData != data.mObjects.end(); objectData++)
	{
		////スポーンポイントを設定
		////指定された名前が入ってるなら対応したやつを入れる
		//if (objectData->spawnpointName == "player")
		//{
		//	testplayer.position = objectData->translation;
		//	testplayer.rotation = objectData->rotation;
		//	testplayer.scale = objectData->scaling;
		//}
		
		//エネミーなら
		//TODO:応急的にeventtrigerを使っているが、今後のことを考えると敵の配置専用の項目を作った方がいい
		if (objectData->eventtrigerName == "enemy")
		{
			EnemyManager::Get()->Load(*objectData);
			continue;
		}

		//イベントオブジェクトなら
		if (objectData->eventtrigerName != "")
		{
			//設置して残りをスキップ
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
		else
		{
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
		obj.Update();
	}

	for (auto& obj : mColObj3ds)
	{
		obj.Update(*Camera::sCamera);
	}
}

void Stage::Draw()
{
	//物によってマテリアル描画とテクスチャ描画が混在してるのに
	//分ける方法を作ってないので作る
	DrawCollider();

	DrawModel();
}

void Stage::NormalObjectSet(const LevelData::ObjectData& data)
{
	//とりあえずキューブで配置
	mObj3ds.emplace_back();
	mObj3ds.back().Initialize();
	//モデル設定
	//ファイルネームが設定されてるならそれで
	if (data.fileName != "")
	{
		//読み込みしてないなら読み込みも行う
		if (ModelManager::GetModel(data.fileName) == nullptr)
		{
			ModelManager::LoadModel(data.fileName, data.fileName);
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
	mEventObjects.emplace_back();
	mEventObjects.back().Initialize();
	mEventObjects.back().trigerName = data.eventtrigerName;

	//バグらないように白テクスチャを入れる
	mEventObjects.back().SetTexture(TextureManager::Get()->GetTexture("white"));
	
	//オブジェクトの配置
	LevelDataExchanger::SetObjectData(mEventObjects.back(), data);

}

void Stage::DrawModel()
{
	if (mShowModel == false) return;
	for (auto& obj : mObj3ds)
	{
		obj.DrawMaterial();
	}
	for (auto& obj : mEventObjects)
	{
		obj.Draw();
	}
}

void Stage::DrawCollider()
{
	if (mShowCollider == false) return;
	for (auto& obj : mColObj3ds)
	{
		obj.DrawMaterial();
	}
}
