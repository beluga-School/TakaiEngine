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
#include "Coin.h"
#include "Dokan.h"
#include "MoveBlock.h"
#include <fstream>
#include <sstream>
#include "StageTitleUI.h"
#include "EventManager.h"

#include "Clear1.h"
#include "GoalCamChange.h"
#include <GameUIManager.h>

void StageChanger::LoadResource()
{
	StageChanger::Get()->goalSystem.LoadResource();
	Dokan::LoadResource();
}

void StageChanger::ChangeLevel(LevelData& data)
{
	if (SceneChange::GetRun() == false)
	{
		//シーン切り替えを開始
		SceneChange::Get()->Start();

		//ステージデータを保存
		currentData = &data;
	}
}

void StageChanger::Initialize(LevelData& data)
{
	//ステージデータを保存
	currentData = &data;

	//ステージ生成
	ChangeUpdate();
}

void StageChanger::Update()
{
	//シーン切り替えが暗転まで到達したら
	if (SceneChange::Get()->IsBlackOut())
	{
		//切り替え開始(同期処理)
		ChangeUpdate();

		//切り替えが終わったら暗転を解除
		SceneChange::Get()->Open();
	}

	//ステージごとに実行されるイベントがあればここで実行
	if (GetNowStageHandle() == "stage_stageselect" &&
		LevelLoader::Get()->GetData("stage_mountain")->isClear &&
		EventManager::Get()->GetEvent("next_1")->get()->isExecuted == false)
	{
		EventManager::Get()->Start("next_1");
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

	for (auto& obj : CollideManager::Get()->allCols)
	{
		obj->Update(*Camera::sCamera);
	}
	for (auto& obj : mGoals)
	{
		obj->Update();
	}

	seaObject->Update();
	goalSystem.Update();
}

void StageChanger::Draw()
{
	//物によってマテリアル描画とテクスチャ描画が混在してるのに
	//分ける方法を作ってないので作る
	BasicObjectPreDraw(PipelineManager::GetPipeLine("Toon"));
	DrawCollider();

	DrawModel();
}

void StageChanger::DrawSprite()
{
	goalSystem.Draw();
}

void StageChanger::Reload()
{
	std::string loadfile = "Scene/";
	loadfile += StageChanger::Get()->GetNowStageHandle();
	LevelLoader::Get()->Load(loadfile, StageChanger::Get()->GetNowStageHandle(), StageChanger::Get()->currentData->mStageNum);

	StageChanger::Get()->ChangeLevel(*LevelLoader::Get()->GetData(StageChanger::Get()->GetNowStageHandle()));
}

std::string StageChanger::GetNowStageHandle()
{
	return currentHandle;
}

LevelData* StageChanger::GetNowStageData()
{
	return LevelLoader::Get()->GetData(GetNowStageHandle());
}

void StageChanger::Reset()
{
	//入ってたものを削除
	mEntitys.clear();
	mEventObjects.clear();
	mGoals.clear();
	CollideManager::Get()->allCols.clear();
	mCannonPoints.clear();
	mMoveBlockEndPoints.clear();
	EnemyManager::Get()->enemyList.clear();

	Player::Get()->Register();

	mTempStarSaves.clear();

	IDdCube::ResetID();

	EventManager::Get()->Clear();
}

void StageChanger::NormalObjectSet(const LevelData::ObjectData& data)
{
	mEntitys.emplace_back();
	mEntitys.back().Initialize();
	mEntitys.back().SetTag(TagTable::Block);

	//コリジョン目的で配置したならオブジェクト配置を行わない
	if (data.fileName == "collision")
	{
		mEntitys.back().SetTag(TagTable::NoDraw);
		return;
	}

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
	
	if (data.textureName != "")
	{
		//指定テクスチャがあるならそっちを使う
		mEntitys.back().SetTexture(TextureManager::Get()->GetTexture(data.textureName));
		mEntitys.back().isTexDraw = true;
	}

	//タイリングの設定
	mEntitys.back().mTiling = data.tiling;
	
	//オブジェクトの配置
	LevelDataExchanger::SetObjectData(mEntitys.back(), data);

	//当たり判定を作成
	if (data.collider.have)
	{
		CollisionSet(data);
	}
}

void StageChanger::CollisionSet(const LevelData::ObjectData& data)
{
	//当たり判定を表示するオブジェクト
	mEntitys.back().box.Initialize();

	//エンティティリストで参照されたくないので、コリジョンのタグを付ける
	mEntitys.back().SetTag(TagTable::Collsion);
	
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

void StageChanger::CollisionSetEvent(const LevelData::ObjectData& data)
{
	//当たり判定を表示するオブジェクト
	mEventObjects.back()->box.Initialize();

	//コリジョンオンリー描画で使うため、コリジョンのタグを付ける
	mEventObjects.back()->SetTag(TagTable::Collsion);

	mEventObjects.back()->box.SetModel(ModelManager::GetModel("BlankCube"));
	mEventObjects.back()->box.SetTexture(TextureManager::Get()->GetTexture("white"));

	mEventObjects.back()->box.position = data.translation + data.collider.center;
	mEventObjects.back()->box.scale = {
		data.scaling.x * data.collider.size.x,
		data.scaling.y * data.collider.size.y,
		data.scaling.z * data.collider.size.z
	};
	mEventObjects.back()->box.rotation = {
		MathF::AngleConvRad(data.rotation.x),
		MathF::AngleConvRad(data.rotation.y),
		MathF::AngleConvRad(data.rotation.z)
	};

	mEventObjects.back()->box.cubecol.position = mEntitys.back().box.position;
	mEventObjects.back()->box.cubecol.scale = mEntitys.back().box.scale;
	//当たり判定だけマネージャーに登録
	mEventObjects.back()->Register();
}

void StageChanger::EvenyObjectSet(const LevelData::ObjectData& data)
{
	std::string tFilename = "";
	if (data.fileName != "")
	{
		//読み込みしてないなら読み込みも行う
		if (ModelManager::GetModel(data.fileName) == nullptr)
		{
			ModelManager::LoadModel(data.fileName, data.fileName, true);
		}
		tFilename = data.fileName;
	}
	
	//stage の文字列が含まれてるなら
	if (data.eventtrigerName.find("stage") != std::string::npos)
	{
		mEventObjects.emplace_back();
		mEventObjects.back() = std::make_unique<WarpBlock>();
		mEventObjects.back()->Initialize();
		//ファイルネームがあるなら
		if (tFilename != "")
		{
			mEventObjects.back()->SetModel(ModelManager::GetModel(tFilename));
		}

		mEventObjects.back()->SetOutLineState({ 1,0,0,1.0f }, 0.05f);

		mEventObjects.back()->trigerName = data.eventtrigerName;
		//バグらないように白テクスチャを入れる
		mEventObjects.back()->SetTexture(TextureManager::Get()->GetTexture("white"));

		//オブジェクトの配置
		LevelDataExchanger::SetObjectData(*mEventObjects.back(), data);

		return;
	}
	//goal の文字列が含まれてるなら
	if (data.eventtrigerName == "goal")
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
		mEventObjects.emplace_back();
		mEventObjects.back() = std::make_unique<Star>();
		mEventObjects.back()->Initialize();
		//ファイルネームがあるなら
		if (tFilename != "")
		{
			mEventObjects.back()->SetModel(ModelManager::GetModel(tFilename));
		}

		mEventObjects.back()->SetOutLineState({ 0,0,0,1.0f }, 0.1f);

		mEventObjects.back()->trigerName = data.eventtrigerName;
		mEventObjects.back()->box.CreateCol(
			mEventObjects.back()->position,
			mEventObjects.back()->scale
		);
		
		//当たり判定を作成
		//starManagerにわけんな一意に定義出来ねえだろ！！！！！!!!!!!1
		if (data.collider.have)
		{
			//当たり判定を表示するオブジェクト
			mEventObjects.back()->box.Initialize();

			//コリジョンオンリー描画で使うため、コリジョンのタグを付ける
			mEventObjects.back()->SetTag(TagTable::Collsion);
			//ブロックのタグを外す
			mEventObjects.back()->DeleteTag(TagTable::Block);

			mEventObjects.back()->box.SetModel(ModelManager::GetModel("BlankCube"));
			mEventObjects.back()->box.SetTexture(TextureManager::Get()->GetTexture("white"));

			mEventObjects.back()->box.position = data.translation + data.collider.center;
			mEventObjects.back()->box.scale = {
				data.scaling.x * data.collider.size.x,
				data.scaling.y * data.collider.size.y,
				data.scaling.z * data.collider.size.z
			};
			mEventObjects.back()->box.rotation = {
				MathF::AngleConvRad(data.rotation.x),
				MathF::AngleConvRad(data.rotation.y),
				MathF::AngleConvRad(data.rotation.z)
			};

			//ここEntitysから引っ張ってきてるけど合ってるのかな
			mEventObjects.back()->box.cubecol.position = mEventObjects.back()->box.position;
			mEventObjects.back()->box.cubecol.scale = mEventObjects.back()->box.scale;
			//当たり判定だけマネージャーに登録
			mEventObjects.back()->Register();
		}

		//オブジェクトの配置
		LevelDataExchanger::SetObjectData(*mEventObjects.back(), data);

		Star* star = static_cast<Star*>(mEventObjects.back().get());

		star->id = atoi(data.setObjectName.c_str());

		mTempStarSaves.push_back(star);

		return;
	}
	//coin の文字列が完全一致するなら
	if (data.eventtrigerName == "coin")
	{
		mEventObjects.emplace_back();
		mEventObjects.back() = std::make_unique<Coin>();
		mEventObjects.back()->Initialize();
		//ファイルネームがあるなら
		if (tFilename != "")
		{
			mEventObjects.back()->SetModel(ModelManager::GetModel(tFilename));
		}

		mEventObjects.back()->SetOutLineState({ 0,0,0,1.0f }, 0.1f);

		mEventObjects.back()->trigerName = data.eventtrigerName;

		//当たり判定を作成
		if (data.collider.have)
		{
			CollisionSetEvent(data);
		}

		//オブジェクトの配置
		LevelDataExchanger::SetObjectData(*mEventObjects.back(), data);

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

void StageChanger::ChangeUpdate()
{
	//ハンドルをステージに保存
	currentHandle = currentData->mHandle;

	Reset();

	for (auto objectData = currentData->mObjects.begin(); objectData != currentData->mObjects.end(); objectData++)
	{
		//カメラの配置なら
		if (objectData->setObjectName == "eventcamera")
		{
			EventCamData camdata;
			camdata.pos = objectData->translation;
			camdata.rotation = objectData->rotation;
			if (objectData->eventtrigerName == "next_1")
			{
				EventManager::Get()->Register<Clear1>(camdata, objectData->eventtrigerName);
			}
			if (objectData->eventtrigerName == "goalCamera")
			{
				EventManager::Get()->Register<GoalCamChange>(camdata, objectData->eventtrigerName);
			}
			if (Util::CheckString(objectData->eventtrigerName,"moveCamera"))
			{

			}
		}

		//海の配置なら
		if (objectData->setObjectName == "sea")
		{
			seaObject = std::make_unique<Sea>();
			seaObject->LoadResource();
			seaObject->Initialize();
			seaObject->SetInfo(
				objectData->translation,
				objectData->scaling,
				objectData->tiling);

			continue;
		}

		//プレイヤーの配置なら
		if (objectData->setObjectName == "player")
		{
			if (playerData.dokanPriority == false)
			{
				playerData.data = *objectData;
				Player::Get()->mDokanApparrance = false;
			}
			
			continue;
		}

		//ここは後で、cannonの時みたいに書式を定義してif分一個にまとめたい
		//エネミーの配置なら
		if (objectData->setObjectName == "enemy")
		{
			EnemyManager::Get()->Load(*objectData);
			
			if (objectData->collider.have)
			{
				//当たり判定を表示するオブジェクト
				EnemyManager::Get()->enemyList.back()->box.Initialize();

				//エンティティリストで参照されたくないので、コリジョンのタグを付ける
				EnemyManager::Get()->enemyList.back()->SetTag(TagTable::Collsion);

				EnemyManager::Get()->enemyList.back()->box.SetModel(ModelManager::GetModel("BlankCube"));
				EnemyManager::Get()->enemyList.back()->box.SetTexture(TextureManager::Get()->GetTexture("white"));

				//中心位置をずらす情報を保存
				EnemyManager::Get()->enemyList.back()->saveColCenter = objectData->collider.center;

				EnemyManager::Get()->enemyList.back()->box.position = objectData->translation + EnemyManager::Get()->enemyList.back()->saveColCenter;
				EnemyManager::Get()->enemyList.back()->box.scale = {
					objectData->scaling.x * objectData->collider.size.x,
					objectData->scaling.y * objectData->collider.size.y,
					objectData->scaling.z * objectData->collider.size.z
				};
				EnemyManager::Get()->enemyList.back()->box.rotation = {
					MathF::AngleConvRad(objectData->rotation.x),
					MathF::AngleConvRad(objectData->rotation.y),
					MathF::AngleConvRad(objectData->rotation.z)
				};

				EnemyManager::Get()->enemyList.back()->box.cubecol.position = EnemyManager::Get()->enemyList.back()->box.position;
				EnemyManager::Get()->enemyList.back()->box.cubecol.scale = EnemyManager::Get()->enemyList.back()->box.scale;
				//当たり判定だけマネージャーに登録
				EnemyManager::Get()->enemyList.back()->Register();
			}

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
			
			if(objectData->collider.have)
			{
				//当たり判定を表示するオブジェクト
				EnemyManager::Get()->enemyList.back()->box.Initialize();

				//エンティティリストで参照されたくないので、コリジョンのタグを付ける
				EnemyManager::Get()->enemyList.back()->SetTag(TagTable::Collsion);

				EnemyManager::Get()->enemyList.back()->box.SetModel(ModelManager::GetModel("BlankCube"));
				EnemyManager::Get()->enemyList.back()->box.SetTexture(TextureManager::Get()->GetTexture("white"));

				//中心位置をずらす情報を保存
				EnemyManager::Get()->enemyList.back()->saveColCenter = objectData->collider.center;

				EnemyManager::Get()->enemyList.back()->box.position = objectData->translation + EnemyManager::Get()->enemyList.back()->saveColCenter;
				EnemyManager::Get()->enemyList.back()->box.scale = {
					objectData->scaling.x * objectData->collider.size.x,
					objectData->scaling.y * objectData->collider.size.y,
					objectData->scaling.z * objectData->collider.size.z
				};
				EnemyManager::Get()->enemyList.back()->box.rotation = {
					MathF::AngleConvRad(objectData->rotation.x),
					MathF::AngleConvRad(objectData->rotation.y),
					MathF::AngleConvRad(objectData->rotation.z)
				};

				EnemyManager::Get()->enemyList.back()->box.cubecol.position = EnemyManager::Get()->enemyList.back()->box.position;
				EnemyManager::Get()->enemyList.back()->box.cubecol.scale = EnemyManager::Get()->enemyList.back()->box.scale;
				//当たり判定だけマネージャーに登録
				EnemyManager::Get()->enemyList.back()->Register();
			}

			continue;
		}

		if (objectData->setObjectName.find("moveBlock") != std::string::npos)
		{
			std::string saveID = "";
			//eventtrigerNameを分解する
			std::vector<std::string> split = Util::SplitString(objectData->eventtrigerName, "_");

			for (auto str : split)
			{
				//数字だけ抜き出す
				if (Util::IsNumber(str))
				{
					saveID = str;
				}
			}

			//endが付いているならID検索で座標を入れて飛ばす
			if (objectData->eventtrigerName.find("end") != std::string::npos)
			{
				mMoveBlockEndPoints.emplace_back();

				mMoveBlockEndPoints.back().key = saveID;
				mMoveBlockEndPoints.back().points = objectData->translation;

				continue;
			}

			//endでないならstartのはずなので、オブジェクトを生成
			mEventObjects.emplace_back();
			mEventObjects.back() = std::make_unique<MoveBlock>();
		
			MoveBlock* mb = dynamic_cast<MoveBlock*>(mEventObjects.back().get());
			
			//初期地点なら位置を代入
			if (objectData->eventtrigerName.find("start") != std::string::npos)
			{
				mb->id = saveID;
				mb->startpos = objectData->translation;
			}

			mEventObjects.back()->Initialize();
			mEventObjects.back()->SetOutLineState({ 0,0,0,1.0f }, 0.1f);

			if (objectData->fileName != "")
			{
				//読み込みしてないなら読み込みも行う
				if (ModelManager::GetModel(objectData->fileName) == nullptr)
				{
					ModelManager::LoadModel(objectData->fileName, objectData->fileName, true);
				}
				mEventObjects.back()->SetModel(ModelManager::GetModel(objectData->fileName));
			}

			//オブジェクトの配置
			LevelDataExchanger::SetObjectData(*mEventObjects.back(), *objectData);

			//当たり判定を作成
			if (objectData->collider.have)
			{
				CollisionSetEvent(*objectData);
			}

			continue;
		}

		//土管を配置
		if (objectData->setObjectName.find("dokan") != std::string::npos)
		{
			mEventObjects.emplace_back();
			mEventObjects.back() = std::make_unique<Dokan>();

			mEventObjects.back()->Initialize();

			mEventObjects.back()->trigerName = objectData->eventtrigerName;

			mEventObjects.back()->SetOutLineState({ 0,0,0,1 }, 0.05f);

			//オブジェクトの配置
			LevelDataExchanger::SetObjectData(*mEventObjects.back(), *objectData);

			if (objectData->collider.have)
			{
				//当たり判定を表示するオブジェクト
				mEventObjects.back()->box.Initialize();

				//コリジョンオンリー描画で使うため、コリジョンのタグを付ける
				mEventObjects.back()->SetTag(TagTable::Collsion);

				mEventObjects.back()->box.SetModel(ModelManager::GetModel("BlankCube"));
				mEventObjects.back()->box.SetTexture(TextureManager::Get()->GetTexture("white"));

				mEventObjects.back()->box.position = objectData->translation + objectData->collider.center;
				mEventObjects.back()->box.scale = {
					objectData->scaling.x * objectData->collider.size.x,
					objectData->scaling.y * objectData->collider.size.y,
					objectData->scaling.z * objectData->collider.size.z
				};
				mEventObjects.back()->box.rotation = {
					MathF::AngleConvRad(objectData->rotation.x),
					MathF::AngleConvRad(objectData->rotation.y),
					MathF::AngleConvRad(objectData->rotation.z)
				};

				mEventObjects.back()->box.cubecol.position = mEventObjects.back()->box.position;
				mEventObjects.back()->box.cubecol.scale = mEventObjects.back()->box.scale;
			}

			//自身の情報を設定する
			std::vector<std::string> split = Util::SplitString(objectData->eventtrigerName, "_");

			Dokan* dokan = dynamic_cast<Dokan*>(mEventObjects.back().get());
			for (auto str : split)
			{
				//移動先の土管IDを取り出す
				if (Util::IsNumber(str))
				{
					dokan->nextDokanInfo.id = atoi(str.c_str());
				}
				//移動先のステージ名を取り出す
				else
				{
					dokan->nextDokanInfo.stageName = str;
				}
			}

			split = Util::SplitString(objectData->setObjectName, "_");
			for (auto str : split)
			{
				//自身の土管IDを取り出す
				if (Util::IsNumber(str))
				{
					dokan->dokanInfo.id = atoi(str.c_str());
				}
			}
			//現在のステージハンドル(ステージ名)を保存して、自身の情報とする
			split = Util::SplitString(currentHandle, "_");

			for (auto str : split)
			{
				//文字列から冠詞の"stage"を取り除いた者を保存
				if (str != "stage")
				{
					dokan->dokanInfo.stageName = str;
				}
			}

			//移動前の土管が持っていた情報と一致する土管が合ったら
			if (saveNextDokanInfo.stageName == dokan->dokanInfo.stageName &&
				saveNextDokanInfo.id == dokan->dokanInfo.id)
			{
				//プレイヤーの情報を記録
				playerData.data = *objectData;
				//優先フラグを立てる
				playerData.dokanPriority = true;

				//ステージ名をUIに記録
				GameUIManager::Get()->GetStageTitleUI()->ChangeHandle(StageChanger::Get()->currentData->mStageNum);
			}

			continue;
		}

		if (objectData->setObjectName == "boardpicture")
		{
			//そのままモデルの配置
			NormalObjectSet(*objectData);

			mEntitys.back().SetModel(ModelManager::GetModel("plate"));
			mEntitys.back().rotation += { 
				0,
				MathF::AngleConvRad(-180.f),
				MathF::AngleConvRad(-90.f),
			};

			//イベントトリガーに記載されたテクスチャ名を自身のテクスチャとして貼る
			//ハンドルがないなら作成
			if (TextureManager::GetTexture(objectData->eventtrigerName) == nullptr)
			{
				std::string filename = "Resources\\";
				filename = filename + objectData->eventtrigerName + ".png";
				TextureManager::Load(filename, objectData->eventtrigerName);
			}
			mEntitys.back().SetTexture(TextureManager::GetTexture(objectData->eventtrigerName));

			//当たり判定を作成
			if (objectData->collider.have)
			{
				CollisionSet(*objectData);
			}

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

		//---ここより前でcontinue忘れると、モデルを読み込んじゃってバグる可能性大

		//なにも無かったら
		{
			//ここまで何もかいてなければ地形の配置として扱う
			//本来はBlock型に変換して入れたいが、ポインタで保持する形になってないのでBlock型にできない
			//なのでTagを内部でBlockを入れる形にして何とかしている

			//そのままモデルの配置
			NormalObjectSet(*objectData);

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
		if (cannon != nullptr)
		{
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
		}

		MoveBlock* moveBlock = dynamic_cast<MoveBlock*>(itr->get());

		if (moveBlock != nullptr)
		{
			//通った場合は、事前に保管しておいた制御点の一覧から
			for (auto itr = mMoveBlockEndPoints.begin(); itr != mMoveBlockEndPoints.end(); itr++)
			{
				//idが一致した場合は入れる
				if (itr->key.find(moveBlock->id) != std::string::npos)
				{
					moveBlock->endpos = itr->points;
				}
			}
		}

		//TODO:事前に保管したデータ群を検索する際、いちいち全検索(線形探索)しているので
		//データの総量が増えると処理時間がのびてしまう アホ
		//探索方を変えない方針で改善するなら、すでに全てのデータが見つかっているデータは
		//スキップするなどの処理が必要だろう
	}

	SetPlayer(playerData.data);
	//土管からの配置でなければ
	if (playerData.dokanPriority == false)
	{
		//現在のステージハンドル(ステージ名)を保存して、自身の情報とする
	 	std::vector<std::string> split = Util::SplitString(currentHandle, "_");

		//データに初期情報を書き込む
		for (auto str : split)
		{
			//文字列から冠詞の"stage"を取り除いた者を保存
			if (str != "stage")
			{
				saveNextDokanInfo.stageName = str;
			}
		}

		saveNextDokanInfo.id = 0;
		Player::Get()->mDokanApparrance = false;
	}
}

void StageChanger::SetPlayer(const LevelData::ObjectData& data)
{
	Player::Get()->Reset();
	Player::Get()->preMove = data.translation;
	Player::Get()->position = data.translation;
	Player::Get()->rotation = data.rotation;

	Player::Get()->box.position = Player::Get()->position;
	Player::Get()->box.scale = Player::Get()->scale;
	Player::Get()->box.cubecol.position = Player::Get()->position;
	Player::Get()->box.cubecol.scale = Player::Get()->scale;

	Player::Get()->mDokanApparrance = true;
}

void StageChanger::DrawModel()
{
	if (mShowModel == false) return;
	for (auto& obj : mEntitys)
	{
		//コリジョン用に配置したオブジェクトならスキップ
		if (obj.CheckTag(TagTable::NoDraw))continue;
		
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
		if (obj.isTexDraw)
		{
			obj.Draw();
		}
		else
		{
			obj.DrawMaterial();
		}
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

	BasicObjectPreDraw(PipelineManager::GetPipeLine("GroundToon"));
	seaObject->Draw();
}

void StageChanger::DrawCollider()
{
	if (mShowCollider == false) return;
	for (auto& obj : mEntitys)
	{
		if (!obj.CheckTag(TagTable::Collsion))continue;

		obj.box.Draw();
	}
	for (auto& obj : mEventObjects)
	{
		if (!obj->CheckTag(TagTable::Collsion))continue;

		obj->box.Draw();
	}
}

void StageChanger::ResetRevise(int32_t stageNumber, int32_t starID, int32_t starnum)
{
	//ファイル出力処理
	std::ofstream writing_file;

	std::string filename = "";
	filename = "./Resources/data/star/";

	std::ostringstream oss;
	oss << stageNumber;

	filename = filename + "stage_" + oss.str() + ".txt";

	writing_file.open(filename, std::ios::out);

	for (int i = 0; i < starnum; i++)
	{
		//取得状況を0(未取得)に書き換える
		if (starID == i)
		{
			int h = 0;
			writing_file << h;
			writing_file << "," << std::endl;
		}
	}

	writing_file.close();
}

void StageChanger::CorrectedRevise(int32_t stageNumber, int32_t starID, int32_t starnum)
{
	//ファイル出力処理
	std::ofstream writing_file;

	std::string filename = "";
	filename = "./Resources/data/star/";

	std::ostringstream oss;
	oss << stageNumber;

	filename = filename + "stage_" +  oss.str() + ".txt";

	writing_file.open(filename, std::ios::out);

	for (int i = 0; i < starnum; i++)
	{
		int h = 0;
		//取得状況を1(取得済)に書き換える
		if (starID == i)
		{
			h = 1;
		}
		writing_file << h;
		writing_file << "," << std::endl;
	}

	writing_file.close();
}

bool StageChanger::LoadStarCorrect(int32_t stageNumber, int32_t starID)
{
	//ファイル入力処理
	std::ifstream reading_file;

	std::string filename = "";
	filename = "./Resources/data/star/";

	std::ostringstream oss;
	oss << stageNumber;

	filename = filename + "stage_" + oss.str() + ".txt";

	reading_file.open(filename);

	//ファイルオープン失敗をチェック
	if (reading_file.fail())
	{
		assert(0);
	}

	std::string line;
	int i = 0;
	while (getline(reading_file, line))
	{
		if(i == starID)
		{
			if (line == "0,")
			{
				return 0;
			}
			if (line == "1,")
			{
				return 1;
			}
		}
		i++;
	}

	return 0;
}
