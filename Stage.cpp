#include "Stage.h"
#include "Model.h"
#include "MathF.h"

void Stage::Load(LevelData& data)
{
	/*auto itr = particles.begin();
	itr != particles.end(); itr++*/
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
		if (objectData->spawnpointName == "enemy")
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
				//当たり判定を表示するオブジェクト
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

				//当たり判定自体の情報を作成
				mColCubes.emplace_back();
				mColCubes.back().position = objectData->translation + objectData->collider.center;
				mColCubes.back().scale = {
					objectData->scaling.x * objectData->collider.size.x,
					objectData->scaling.y * objectData->collider.size.y,
					objectData->scaling.z * objectData->collider.size.z
				};
			}
		}
	}
}

void Stage::Update()
{
	for (auto &obj: mObj3ds)
	{
		obj.Update(*Camera::sCamera);
	}
}

void Stage::Draw()
{
	//物によってマテリアル描画とテクスチャ描画が混在してるのに
	//分ける方法を作ってないので作る
	for (auto& obj : mObj3ds)
	{
		if (obj.MODEL->mSaveModelname == "BlankCube")
		{
		}
		obj.DrawMaterial();
	}
}
