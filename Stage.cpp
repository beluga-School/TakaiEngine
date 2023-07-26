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
#include "Bombking.h"

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

void Stage::Initialize(LevelData& data)
{
	//ステージデータを保存
	currentData = &data;

	//ステージ生成
	ChangeUpdate();
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

	for (auto& obj : mEntitys)
	{
		obj.Update(*Camera::sCamera);
		obj.box.Update(*Camera::sCamera);
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

	for (auto& obj : CollideManager::Get()->allCols)
	{
		obj->Update(*Camera::sCamera);
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
	mEntitys.emplace_back();
	mEntitys.back().Initialize();

	mEntitys.back().taglist.push_back(TagTable::Block);

	//アウトライン設定
	mEntitys.back().SetOutLineState({ 0,0,0,1.0f }, 0.05f);

	//モデル設定
	//ファイルネームが設定されてるならそれで
	if (data.fileName != "")
	{
		//読み込みしてないなら読み込みも行う
		if (ModelManager::GetModel(data.fileName) == nullptr)
		{
			ModelManager::LoadModel(data.fileName, data.fileName,true);
		}
		mEntitys.back().SetModel(ModelManager::GetModel(data.fileName));
	}
	//ないなら四角をデフォで設定
	else
	{
		mEntitys.back().SetModel(ModelManager::GetModel("Cube"));
	}

	//バグらないように白テクスチャを入れる
	mEntitys.back().SetTexture(TextureManager::Get()->GetTexture("white"));
	
	//オブジェクトの配置
	LevelDataExchanger::SetObjectData(mEntitys.back(), data);
}

void Stage::CollisionSet(const LevelData::ObjectData& data)
{
	//当たり判定を表示するオブジェクト
	mEntitys.back().box.Initialize();

	//エンティティリストで参照されたくないので、コリジョンのタグを付ける
	mEntitys.back().taglist.push_back(TagTable::Collsion);
	
	mEntitys.back().box.SetModel(ModelManager::GetModel("BlankCube"));
	mEntitys.back().box.SetTexture(TextureManager::Get()->GetTexture("white"));

	mEntitys.back().box.position = data.translation + data.collider.center;
	mEntitys.back().box.scale = {
		data.scaling.x * data.collider.size.x,
		data.scaling.y * data.collider.size.y,
		data.scaling.z * data.collider.size.z
	};
	mEntitys.back().box.rotation = {
		MathF::AngleConvRad(data.rotation.x),
		MathF::AngleConvRad(data.rotation.y),
		MathF::AngleConvRad(data.rotation.z)
	};

	mEntitys.back().box.cubecol.position = mEntitys.back().box.position;
	mEntitys.back().box.cubecol.scale = mEntitys.back().box.scale;
	//当たり判定だけマネージャーに登録
	mEntitys.back().Register();
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
		//全てのオブジェクト配置後、制御点と大砲のみで比較しなおすため、
		//値を一時的に保存しておく
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
	mEntitys.clear();
	mEventObjects.clear();
	mGoals.clear();
	StarManager::Get()->mStars.clear();
	CollideManager::Get()->allCols.clear();
	mCannonPoints.clear();
	EnemyManager::Get()->enemyList.clear();

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

		//ここは後で、cannonの時みたいに書式を定義してif分一個にまとめたい
		//エネミーの配置なら
		if (objectData->setObjectName == "enemy")
		{
			EnemyManager::Get()->Load(*objectData);
			continue;
		}
		//ボスの配置なら
		if (objectData->setObjectName == "bombking")
		{
			EnemyManager::Get()->enemyList.emplace_back();
			EnemyManager::Get()->enemyList.back() = std::make_unique<Bombking>();

			//読み込みしてないなら読み込みも行う
			if (ModelManager::GetModel(objectData->fileName) == nullptr)
			{
				ModelManager::LoadModel(objectData->fileName, objectData->fileName, true);
			}

			//モデルとか設定する
			EnemyManager::Get()->enemyList.back()->Initialize();

			//positionとかを設定
			LevelDataExchanger::SetObjectData(*EnemyManager::Get()->enemyList.back(), *objectData);
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

		//---ここより前でcontinue忘れると、モデルを読み込んじゃってバグる可能性大

		//なにも無かったら
		{
			//ここまで何もかいてなければ地形の配置として扱う
			//本来はBlock型に変換して入れたいが、ポインタで保持する形になってないのでBlock型にできない
			//なのでTagを内部でBlockを入れる形にして何とかしている

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
	for (auto& obj : mEntitys)
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
	for (auto& obj : mEntitys)
	{
		if (!obj.CheckTag(TagTable::Collsion))continue;

		obj.box.Draw();
	}
}
