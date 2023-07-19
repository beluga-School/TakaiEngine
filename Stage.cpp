#include "Stage.h"
#include "Model.h"
#include "MathF.h"
#include "EnemyManager.h"
#include "Player.h"
#include "WarpBlock.h"
#include "ClearDrawScreen.h"
#include "Star.h"
#include "SceneChange.h"
#include "Cannon.h"
#include <sstream>

void Stage::ChangeLevel(LevelData& data)
{
	if (SceneChange::GetRun() == false)
	{
		//シーン切り替えを開始
		SceneChange::Get()->Start();

		//ステージデータを保存
		currentData = &data;
	}
}

void Stage::Update()
{
	//シーン切り替えが暗転まで到達したら
	if (SceneChange::Get()->IsBlackOut())
	{
		//切り替え開始(同期処理)
		ChangeUpdate();

		//切り替えが終わったら暗転を解除
		SceneChange::Get()->Open();
	}

	for (auto &obj: mObj3ds)
	{
		obj.Update(*Camera::sCamera);
	}

	for (auto& obj : mEventObjects)
	{
		obj->Update();
	}

	for (auto& obj : StarManager::Get()->mStars)
	{
		obj->Update();
	}
	StarManager::Get()->Update();

	for (auto& obj : mColObj3ds)
	{
		obj.Update(*Camera::sCamera);
		obj.CreateCol();
	}
	for (auto& obj : mGoals)
	{
		obj->Update();
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

void Stage::Reload()
{
	std::string loadfile = "Scene/";
	loadfile += Stage::Get()->GetNowStageHandle();
	LevelLoader::Get()->Load(loadfile, Stage::Get()->GetNowStageHandle());

	Stage::Get()->ChangeLevel(*LevelLoader::Get()->GetData(Stage::Get()->GetNowStageHandle()));
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
	mObj3ds.back().SetOutLineState({ 0,0,0,1.0f }, 0.05f);

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

		mEventObjects.back()->SetOutLineState({ 1,0,0,1.0f }, 0.05f);

		mEventObjects.back()->trigerName = data.eventtrigerName;
		//バグらないように白テクスチャを入れる
		mEventObjects.back()->SetTexture(TextureManager::Get()->GetTexture("white"));

		//オブジェクトの配置
		LevelDataExchanger::SetObjectData(*mEventObjects.back(), data);

		return;
	}
	//goal の文字列が含まれてるなら
	if (data.eventtrigerName.find("goal") != std::string::npos)
	{
		mGoals.emplace_back();
		mGoals.back() = std::make_unique<Goal>();
		mGoals.back()->Initialize();
		mGoals.back()->trigerName = data.eventtrigerName;

		mGoals.back()->SetOutLineState({ 1,0,0,1.0f }, 0.05f);

		//オブジェクトの配置
		LevelDataExchanger::SetObjectData(*mGoals.back(), data);

		return;
	}
	//star の文字列が完全一致するなら
	if (data.eventtrigerName == "star")
	{
		StarManager::Get()->mStars.emplace_back();
		StarManager::Get()->mStars.back() = std::make_unique<Star>();
		StarManager::Get()->mStars.back()->Initialize();

		StarManager::Get()->mStars.back()->SetOutLineState({ 0,0,0,1.0f }, 0.1f);

		StarManager::Get()->mStars.back()->trigerName = data.eventtrigerName;
		
		//オブジェクトの配置
		LevelDataExchanger::SetObjectData(*StarManager::Get()->mStars.back(), data);

		return;
	}
	//Cannon の文字列が含まれてるなら
	if (data.eventtrigerName.find("Cannon") != std::string::npos)
	{
		if (data.eventtrigerName.find("start") != std::string::npos)
		{
			std::stringstream ss;

			mEventObjects.emplace_back();
			mEventObjects.back() = std::make_unique<Cannon>();
			mEventObjects.back()->Initialize();

			std::vector<std::string> split = Util::SplitString(data.eventtrigerName, "_");

			//イベントトリガー名をstringstreamに代入
			for (auto str : split)
			{
				//数字だけ抜き出す
				if(Util::IsNumber(str)) ss << str;
			}

			Cannon* cannon = static_cast<Cannon*>(mEventObjects.back().get());
			
			//int32_t型に出力する
			//この際、文字列から数値のみが出力される
			ss >> cannon->id;
			cannon->startPos = data.translation;

			//オブジェクトの配置
			LevelDataExchanger::SetObjectData(*mEventObjects.back(), data);
		}
		if (data.eventtrigerName.find("inter") != std::string::npos)
		{	
			CannonPoint point = { data.eventtrigerName, data.translation };
			mCannonPoints.push_back(point);
		}
		if (data.eventtrigerName.find("end") != std::string::npos)
		{
			CannonPoint point = { data.eventtrigerName, data.translation };
			mCannonPoints.push_back(point);
		}

		return;
	}
}

void Stage::ChangeUpdate()
{
	//ハンドルをステージに保存
	currentHandle = currentData->mHandle;

	//入ってたものを削除
	mObj3ds.clear();
	mColCubes.clear();
	mEventObjects.clear();
	mColObj3ds.clear();
	mGoals.clear();
	StarManager::Get()->mStars.clear();
	CollideManager::Get()->allCols.clear();
	mCannonPoints.clear();

	Player::Get()->Register();

	for (auto objectData = currentData->mObjects.begin(); objectData != currentData->mObjects.end(); objectData++)
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

			////当たり判定を作成
			//if (objectData->collider.have)
			//{
			//	CollisionSet(*objectData);
			//}

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
			
				//コリジョンを取るオブジェクトへのポインタを保持
				mColObj3ds.back().collideObj = &mObj3ds.back();
			}

			continue;
		}
	}

	//全てのイベントオブジェクトを検索して
	for (auto itr = mEventObjects.begin(); itr != mEventObjects.end(); itr++)
	{
		//Cannon型にキャスト
		//dynamic_castの仕様で、Cannonでなければnullptrと判定されるので
		Cannon* cannon = dynamic_cast<Cannon*>(itr->get());
		//異なった場合次へ
		if (cannon == nullptr)continue;

		//通った場合は、事前に保管しておいた制御点の一覧から
		for (auto itr = mCannonPoints.begin(); itr != mCannonPoints.end(); itr++)
		{
			//中間点と
			if (itr->key.find("inter") != std::string::npos)
			{
				//idが一致した場合は入れる
				if (itr->key.find(cannon->id) != std::string::npos)
				{
					cannon->interPos = itr->points;
				}
			}
			//最終点を判別する
			if (itr->key.find("end") != std::string::npos)
			{
				//idが一致した場合は入れる
				if (itr->key.find(cannon->id) != std::string::npos)
				{
					cannon->endPos = itr->points;
				}
			}
		}

		//TODO:事前に保管したデータ群を検索する際、いちいち全検索(線形探索)しているので
		//データの総量が増えると処理時間がのびてしまう アホ
		//探索方を変えない方針で改善するなら、すでに全てのデータが見つかっているデータは
		//スキップするなどの処理が必要だろう
	}
}

void Stage::DrawModel()
{
	if (mShowModel == false) return;
	for (auto& obj : mObj3ds)
	{
		BasicObjectPreDraw(PipelineManager::GetPipeLine("OutLine"), false);
		obj.DrawOutLine();
		//アルファが1未満になるなら透明用描画パイプラインに切り替える
		if (obj.color_.w < 1.0f)
		{
			BasicObjectPreDraw(PipelineManager::GetPipeLine("GroundToonNDW"));
		}
		else
		{
			BasicObjectPreDraw(PipelineManager::GetPipeLine("GroundToon"));
		}
		obj.DrawMaterial();
	}
	for (auto& obj : mEventObjects)
	{
		BasicObjectPreDraw(PipelineManager::GetPipeLine("OutLine"),false);
		obj->DrawOutLine();
		//アルファが1未満になるなら透明用描画パイプラインに切り替える
		if (obj->color_.w < 1.0f)
		{
			BasicObjectPreDraw(PipelineManager::GetPipeLine("GroundToonNDW"));
		}
		else
		{
			BasicObjectPreDraw(PipelineManager::GetPipeLine("GroundToon"));
		}
		obj->Draw();
	}
	for (auto& obj : StarManager::Get()->mStars)
	{
		BasicObjectPreDraw(PipelineManager::GetPipeLine("OutLine"), false);
		obj->DrawOutLine();
		//アルファが1未満になるなら透明用描画パイプラインに切り替える
		if (obj->color_.w < 1.0f)
		{
			BasicObjectPreDraw(PipelineManager::GetPipeLine("GroundToonNDW"));
		}
		else
		{
			BasicObjectPreDraw(PipelineManager::GetPipeLine("GroundToon"));
		}
		obj->Draw();
	}

	for (auto& obj : mGoals)
	{
		BasicObjectPreDraw(PipelineManager::GetPipeLine("OutLine"), false);
		obj->DrawOutLine();
		//アルファが1未満になるなら透明用描画パイプラインに切り替える
		if (obj->color_.w < 1.0f)
		{
			BasicObjectPreDraw(PipelineManager::GetPipeLine("ToonNDW"));
		}
		else
		{
			BasicObjectPreDraw(PipelineManager::GetPipeLine("Toon"));
		}
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
