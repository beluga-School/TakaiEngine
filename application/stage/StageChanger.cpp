#include "StageChanger.h"
#include "Model.h"
#include "MathF.h"
#include "EnemyManager.h"
#include "Player.h"
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
#include "BossArea.h"

#include "Clear1.h"
#include "NoEffectEvent.h"
#include <GameUIManager.h>
#include <EventCameraManager.h>
#include <EventTriggerBox.h>
#include <StageTitleUIMountain.h>
#include "Signboard.h"
#include <EnemyDokan.h>
#include "SummonUI.h"
#include "BossHikiCamera.h"
#include "EnemyPopEvent.h"
#include "GetStar.h"
#include "StarStringEvent.h"
#include "RedCoin.h"
#include "RedCoinEvent.h"
#include "Slime.h"
#include "BombSolider.h"
#include "BreakBlock.h"
#include "DropBlock.h"
#include "TikuwaBlock.h"

void StageChanger::LoadResource()
{
	StageChanger::Get()->goalSystem.LoadResource();
	Dokan::LoadResource();
	EnemyDokan::LoadResource();
	Signboard::LoadResource();
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

	//クリアイベントの実行
	if (GetNowStageHandle() == "stage_mountain")
	{
		//スターの残数を監視し、0より小さいならクリアイベント実行
		if (GameUIManager::Get()->starUI.GetCount() <= 0) {
			EventManager::Get()->Start("clearEvent");
		}
	}

	for (auto& obj : mEntitys)
	{
		if (!obj->mActive)continue;

		obj->Update();
	}

	for (auto& obj : CollideManager::Get()->allCols)
	{
		if (!obj->mActive)continue;

		obj->box.Update(*Camera::sCamera);
		obj->box.CreateCol(obj->box.position, obj->box.scale, obj->box.rotation);
	}

	seaObject->Update();
	goalSystem.Update();
}

void StageChanger::Draw()
{
	//物によってマテリアル描画とテクスチャ描画が混在してるのに
	//分ける方法を作ってないので作る
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
	CollideManager::Get()->allCols.clear();
	mCannonPoints.clear();
	mMoveBlockEndPoints.clear();
	EnemyManager::Get()->enemyList.clear();

	Player::Get()->Register();

	IDdCube::ResetID();

	EventManager::Get()->Clear();
	EventManager::Get()->Initialize();
	GameUIManager::Get()->Initialize();

	eventCameraNames.clear();
	loadCamDatas.clear();
	loadTargetDatas.clear();
	EventCameraManager::Get()->Reset();

	//入ったときのステージ名がステージセレクト以外なら
	if (!(GetNowStageHandle() == "stage_stageselect"))
	{
		//カメライベントの実行フラグを戻す
		EventManager::Get()->CamFlagReset();
	}

	LightGroup::Get()->Reset();
}

bool StageChanger::SetBlock(const LevelData::ObjectData& data)
{
	SetObject<Block>(data);

	//当たり判定を作成
	if (data.collider.have)
	{
		CollisionSet(data);
	}

	return true;
}

void StageChanger::CollisionSet(const LevelData::ObjectData& data)
{
	//当たり判定を表示するオブジェクト
	mEntitys.back()->box.Initialize();

	//エンティティリストで参照されたくないので、コリジョンのタグを付ける
	mEntitys.back()->SetTag(TagTable::Collsion);
	
	mEntitys.back()->box.SetModel(ModelManager::GetModel("Cube"));
	mEntitys.back()->box.SetTexture(TextureManager::Get()->GetTexture("white"));

	mEntitys.back()->box.position = data.translation + data.collider.center;
	mEntitys.back()->box.scale = {
		data.scaling.x * data.collider.size.x,
		data.scaling.y * data.collider.size.y,
		data.scaling.z * data.collider.size.z
	};
	mEntitys.back()->box.rotation = {
		MathF::AngleConvRad(data.rotation.x),
		MathF::AngleConvRad(data.rotation.y),
		MathF::AngleConvRad(data.rotation.z)
	};

	mEntitys.back()->box.cubecol.position = mEntitys.back()->box.position;
	mEntitys.back()->box.cubecol.scale = mEntitys.back()->box.scale;
	//当たり判定だけマネージャーに登録
	mEntitys.back()->Register();
}

void StageChanger::EnemyCollisionSet(const LevelData::ObjectData& data)
{
	//当たり判定を表示するオブジェクト
	EnemyManager::Get()->enemyList.back()->box.Initialize();

	//エンティティリストで参照されたくないので、コリジョンのタグを付ける
	EnemyManager::Get()->enemyList.back()->SetTag(TagTable::Collsion);

	EnemyManager::Get()->enemyList.back()->box.SetModel(ModelManager::GetModel("Cube"));
	EnemyManager::Get()->enemyList.back()->box.SetTexture(TextureManager::Get()->GetTexture("white"));

	//中心位置をずらす情報を保存
	EnemyManager::Get()->enemyList.back()->saveColCenter = data.collider.center;

	EnemyManager::Get()->enemyList.back()->box.position = data.translation + EnemyManager::Get()->enemyList.back()->saveColCenter;
	EnemyManager::Get()->enemyList.back()->box.scale = {
		data.scaling.x * data.collider.size.x,
		data.scaling.y * data.collider.size.y,
		data.scaling.z * data.collider.size.z
	};
	EnemyManager::Get()->enemyList.back()->box.rotation = {
		MathF::AngleConvRad(data.rotation.x),
		MathF::AngleConvRad(data.rotation.y),
		MathF::AngleConvRad(data.rotation.z)
	};

	EnemyManager::Get()->enemyList.back()->box.cubecol.position = EnemyManager::Get()->enemyList.back()->box.position;
	EnemyManager::Get()->enemyList.back()->box.cubecol.scale = EnemyManager::Get()->enemyList.back()->box.scale;
	//当たり判定だけマネージャーに登録
	EnemyManager::Get()->enemyList.back()->Register();
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
			CameraLoader<NoEffectEvent>(*objectData,"goalCamera");
			CameraLoader<NoEffectEvent>(*objectData,"startCamera");
			CameraLoader<NoEffectEvent>(*objectData, "lockbackCam");
			CameraLoader<EnemyPopEvent>(*objectData, "enemyPopEvent_Slime");
			CameraLoader<MessageEvent>(*objectData, "StarStringEvent");
			//テクスチャを外部から設定
			if (EventManager::Get()->CheckExestEvent("StarStringEvent") != nullptr) {
				MessageEvent* getEvent = static_cast<MessageEvent*>( EventManager::Get()->CheckExestEvent("StarStringEvent")->get());
				getEvent->messageTexHandle = "getstarstring";
			}
			CameraLoader<MessageEvent>(*objectData, "RedStringEvent");
			//テクスチャを外部から設定
			if (EventManager::Get()->CheckExestEvent("RedStringEvent") != nullptr) {
				MessageEvent* getEvent = static_cast<MessageEvent*>(EventManager::Get()->CheckExestEvent("RedStringEvent")->get());
				getEvent->messageTexHandle = "redcoinString";
			}
			CameraLoader<MessageEvent>(*objectData, "RedEventEnd");

			continue;
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
			EnemyManager::Get()->enemyList.back()->SetInitScale(objectData->scaling);

			Mob* enemy = static_cast<Mob*>(EnemyManager::Get()->enemyList.back().get());
			enemy->SetShadow();
			
			if (objectData->collider.have)
			{
				//当たり判定を表示するオブジェクト
				EnemyManager::Get()->enemyList.back()->box.Initialize();

				//エンティティリストで参照されたくないので、コリジョンのタグを付ける
				EnemyManager::Get()->enemyList.back()->SetTag(TagTable::Collsion);

				EnemyManager::Get()->enemyList.back()->box.SetModel(ModelManager::GetModel("Cube"));
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

			//positionとかを設定
			LevelDataExchanger::SetObjectData(*EnemyManager::Get()->enemyList.back(), *objectData);

			//モデルとか設定する
			EnemyManager::Get()->enemyList.back()->Initialize();
			EnemyManager::Get()->enemyList.back()->SetInitScale(objectData->scaling);

			if(objectData->collider.have)
			{
				EnemyCollisionSet(*objectData);
			}

			continue;
		}

		//
		if (objectData->setObjectName == "bombSolider")
		{
			EnemyManager::Get()->enemyList.emplace_back();
			EnemyManager::Get()->enemyList.back() = std::make_unique<BombSolider>();

			Mob* enemy = static_cast<Mob*>(EnemyManager::Get()->enemyList.back().get());
			enemy->SetShadow();

			//読み込みしてないなら読み込みも行う
			if (ModelManager::GetModel(objectData->fileName) == nullptr)
			{
				ModelManager::LoadModel(objectData->fileName, objectData->fileName, true);
			}

			//positionとかを設定
			LevelDataExchanger::SetObjectData(*EnemyManager::Get()->enemyList.back(), *objectData);

			//モデルとか設定する
			EnemyManager::Get()->enemyList.back()->Initialize();
			EnemyManager::Get()->enemyList.back()->SetInitScale(objectData->scaling);

			if (objectData->collider.have)
			{
				EnemyCollisionSet(*objectData);
			}

			continue;
		}

		//移動ブロックの配置
		if (SetMoveBlock(*objectData))
		{
			continue;
		}

		//土管を配置
		if (SetDokan(*objectData))
		{
			continue;
		}
		//敵が出てくる土管を配置
		if (SetEnemyDokan(*objectData))
		{
			continue;
		}

		if (SetSignBoard(*objectData))
		{
			continue;
		}

		//イベント実施オブジェクトの配置
		if (SetEventTrigger(*objectData))
		{
			continue;
		}

		//goal の文字列が含まれてるなら
		if (SetGoal(*objectData))
		{
			continue;
		}

		//star の文字列が完全一致するなら
		if (SetStar(*objectData))
		{
			continue;
		}

		//Cannon の文字列が含まれてるなら
		if (SetCannon(*objectData))
		{
			continue;
		}

		if (SetBossArea(*objectData))
		{
			continue;
		}

		if (SetRedCoin(*objectData))
		{
			continue;
		}
		if (SetBreakBlock(*objectData))
		{
			continue;
		}

		if (SetDropBlock(*objectData))
		{
			continue;
		}

		if (SetTikuwaBlock(*objectData))
		{
			continue;
		}

		if(SetBlock(*objectData))
		{
			continue;
		}
	}

	//全てのオブジェクトを検索して
	for (auto itr = mEntitys.begin(); itr != mEntitys.end(); itr++)
	{
		//Cannon型にキャスト
		//dynamic_castの仕様で、Cannonでなければnullptrと判定されるので
		Cannon* cannon = dynamic_cast<Cannon*>(itr->get());
		//異なった場合次へ
		if (cannon != nullptr)
		{
			//通った場合は、事前に保管しておいた制御点の一覧から
			for (auto itr2 = mCannonPoints.begin(); itr2 != mCannonPoints.end(); itr2++)
			{
				//中間点と
				if (itr2->key.find("inter") != std::string::npos)
				{
					//idが一致した場合は入れる
					if (itr2->key.find(cannon->id) != std::string::npos)
					{
						cannon->interPos = itr2->points;
					}
				}
				//最終点を判別する
				if (itr2->key.find("end") != std::string::npos)
				{
					//idが一致した場合は入れる
					if (itr2->key.find(cannon->id) != std::string::npos)
					{
						cannon->endPos = itr2->points;
					}
				}
			}
		}

		//移動ブロックも大砲と同様の処理をおこなう
		MoveBlock* moveBlock = dynamic_cast<MoveBlock*>(itr->get());

		if (moveBlock != nullptr)
		{
			//通った場合は、事前に保管しておいた制御点の一覧から
			for (auto itr3 = mMoveBlockEndPoints.begin(); itr3 != mMoveBlockEndPoints.end(); itr3++)
			{
				//idが一致した場合は入れる
				if (itr3->key.find(moveBlock->id) != std::string::npos)
				{
					moveBlock->endpos = itr3->points;
				}
			}
		}
	}

	//全てのカメラオブジェクトを検索して
	for (auto itr = eventCameraNames.begin(); itr != eventCameraNames.end(); itr++)
	{
		//並び替える
		std::stable_sort(loadCamDatas[*itr].begin(), loadCamDatas[*itr].end(), [](const LoadCamData& lh, const LoadCamData& rh) {
		return lh.eventnumber < rh.eventnumber;
			});

		EventCamManageData datas;
		//入れたいデータだけ取り出して
		for (auto& data : loadCamDatas[*itr])
		{
			datas.datas.push_back(data.camData);
		}

		//ターゲットがあるか確認
		for (auto itr2 = loadTargetDatas.begin(); itr2 != loadTargetDatas.end(); itr2++)
		{
			//イベント名が一致するものが有ったら
			if (*itr == itr2->eventname)
			{
				//ターゲットを入れる
				datas.target = itr2->target;
				break;
			}
		}

		//一旦手打ち　Blenderのカスタムプロパティが個別対応に使いやすそうだったので、それから読み込む仕組みに後で変える
		if (*itr == "startCamera")
		{
			datas.SetTimersInfo(10.0f, 3.0f, 3.0f);
		}
		if (*itr == "goalCamera")
		{
			datas.SetTimersInfo(3.0f, 1.0f, 1.0f);
		}

		//イベント名と並び替えたカメラデータが入る
		EventCameraManager::Get()->Register(*itr, datas);
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
	Player::Get()->mPreMove = data.translation;
	Player::Get()->position = data.translation;
	Player::Get()->rotation = data.rotation;

	Player::Get()->box.SetTexture(TextureManager::GetTexture("white"));
	Player::Get()->box.SetModel(ModelManager::GetModel("Cube"));

	Player::Get()->box.position = Player::Get()->position;
	Player::Get()->box.scale = Player::Get()->scale;
	Player::Get()->box.cubecol.position = Player::Get()->position;
	Player::Get()->box.cubecol.scale = Player::Get()->scale;

	Player::Get()->mDokanApparrance = true;

	Player::Get()->Initialize();

	Mob* mob = static_cast<Mob*>(Player::Get());
	mob->SetShadow();

	Player::Get()->Register();
}

bool StageChanger::SetDokan(const LevelData::ObjectData& data)
{
	if (Util::CheckString(data.setObjectName, "dokan"))
	{
		SetObject<Dokan>(data);

		mEntitys.back()->SetTag(TagTable::Block);

		if (data.collider.have)
		{
			CollisionSet(data);
		}

		//自身の情報を設定する
		std::vector<std::string> split = Util::SplitString(data.eventtrigerName, "_");

		Dokan* dokan = static_cast<Dokan*>(mEntitys.back().get());
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

		split = Util::SplitString(data.setObjectName, "_");
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
			playerData.data = data;
			//優先フラグを立てる
			playerData.dokanPriority = true;

			//ステージ名をUIに記録
			GameUIManager::Get()->GetStageTitleUI()->ChangeHandle(StageChanger::Get()->currentData->mStageNum);
		}

		return true;
	}
	return false;
}

bool StageChanger::SetEnemyDokan(const LevelData::ObjectData& data)
{
	
	if (Util::CheckString(data.setObjectName, "enemy_spawnpoint"))
	{
		SetObject<EnemyDokan>(data);

		mEntitys.back()->Register();

		//今はエネミーが一種類なのでこれで済ませるが、後々エネミーの種類を管理するマネージャーから名前検索をする形で取得する
		if (data.eventtrigerName == "slime")
		{

		}

		//当たり判定を作成
		if (data.collider.have)
		{
			CollisionSet(data);
		}
		return true;
	}
	return false;
}

bool StageChanger::SetStar(const LevelData::ObjectData& data)
{
	if (data.setObjectName == "star")
	{
		SetObject<Star>(data);

		//当たり判定を作成
		if (data.collider.have)
		{
			CollisionSet(data);
		}

		Star* star = static_cast<Star*>(mEntitys.back().get());

		star->id = -1;

		//イベント名に何か入ってたら
		if (data.eventtrigerName != "") {
			star->mActive = false;
		}

		GameUIManager::Get()->starUI.CountUp();

		return true;
	}
	//star の文字列が完全一致するなら
	if (data.eventtrigerName == "star")
	{
		SetObject<Star>(data);

		//当たり判定を作成
		if (data.collider.have)
		{
			CollisionSet(data);
		}

		Star* star = static_cast<Star*>(mEntitys.back().get());

		star->id = atoi(data.setObjectName.c_str());

		GameUIManager::Get()->starUI.CountUp();

		return true;
	}
	return false;
}

bool StageChanger::SetCannon(const LevelData::ObjectData& data)
{
	if (data.eventtrigerName.find("Cannon") != std::string::npos)
	{
		if (data.eventtrigerName.find("start") != std::string::npos)
		{
			SetObject<Cannon>(data);

			if (data.collider.have)
			{
				CollisionSet(data);
			}

			Cannon* cannon = static_cast<Cannon*>(mEntitys.back().get());
			
			std::stringstream ss;
			std::vector<std::string> split = Util::SplitString(data.eventtrigerName, "_");

			//イベントトリガー名をstringstreamに代入
			for (auto str : split)
			{
				//数字だけ抜き出す
				if (Util::IsNumber(str)) ss << str;
			}

			//int32_t型に出力する
			//この際、文字列から数値のみが出力される
			ss >> cannon->id;
			cannon->startPos = data.translation;
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

		return true;
	}
	return false;
}

bool StageChanger::SetMoveBlock(const LevelData::ObjectData& data)
{
	if (data.setObjectName.find("moveBlock") != std::string::npos)
	{
		std::string saveID = "";
		//eventtrigerNameを分解する
		std::vector<std::string> split = Util::SplitString(data.eventtrigerName, "_");

		for (auto str : split)
		{
			//数字だけ抜き出す
			if (Util::IsNumber(str))
			{
				saveID = str;
			}
		}

		//endが付いているならID検索で座標を入れて飛ばす
		if (data.eventtrigerName.find("end") != std::string::npos)
		{
			mMoveBlockEndPoints.emplace_back();

			mMoveBlockEndPoints.back().key = saveID;
			mMoveBlockEndPoints.back().points = data.translation;

			return true;
		}

		SetObject<MoveBlock>(data);

		mEntitys.back()->SetTag(TagTable::Block);

		//当たり判定を作成
		if (data.collider.have)
		{
			CollisionSet(data);
		}

		MoveBlock* mb = dynamic_cast<MoveBlock*>(mEntitys.back().get());

		//初期地点なら位置を代入
		if (data.eventtrigerName.find("start") != std::string::npos)
		{
			mb->id = saveID;
			mb->startpos = data.translation;
		}

		//static付きならフラグを立てる
		if (data.setObjectName.find("static") != std::string::npos)
		{
			mb->SetStatic();
		}

		mb->TimerStart();

		return true;
	}
	return false;
}

bool StageChanger::SetEventTrigger(const LevelData::ObjectData& data)
{
	//イベント実施オブジェクトの配置
	if (Util::CheckString(data.setObjectName, "triggerBox"))
	{
		mEntitys.emplace_back();
		mEntitys.back() = std::make_unique<EventTriggerBox>();
		mEntitys.back()->Initialize();

		//コリジョン目的で配置したならオブジェクト配置を行わない
		mEntitys.back()->SetTag(TagTable::NoDraw);

		//実行したいイベント名を保持
		EventTriggerBox* etb = static_cast<EventTriggerBox*>(mEntitys.back().get());
		etb->eventName_ = data.eventtrigerName;

		//イベントマネージャーにも登録
		RegisterEvent(etb->eventName_);

		//当たり判定を作成
		if (data.collider.have)
		{
			CollisionSet(data);
		}

		return true;
	}
	return false;
}

bool StageChanger::SetSignBoard(const LevelData::ObjectData& data)
{
	//看板設置
	if (data.setObjectName == "boardpicture")
	{
		//そのままモデルの配置
		SetBlock(data);

		mEntitys.back()->SetModel(ModelManager::GetModel("plate"));
		mEntitys.back()->rotation += {
			0,
				MathF::AngleConvRad(-180.f),
				MathF::AngleConvRad(-90.f),
		};

		//イベントトリガーに記載されたテクスチャ名を自身のテクスチャとして貼る
		//ハンドルがないなら作成
		if (TextureManager::GetTexture(data.eventtrigerName) == nullptr)
		{
			std::string filename = "Resources\\";
			filename = filename + data.eventtrigerName + ".png";
			TextureManager::Load(filename, data.eventtrigerName);
		}
		mEntitys.back()->SetTexture(TextureManager::GetTexture(data.eventtrigerName));

		//当たり判定を作成
		if (data.collider.have)
		{
			CollisionSet(data);
		}

		return true;
	}

	if (data.setObjectName == "signboard")
	{
		SetObject<Signboard>(data);
		//当たり判定を作成
		if (data.collider.have)
		{
			CollisionSet(data);
		}

		Signboard* signboard = static_cast<Signboard*>(mEntitys.back().get());

		signboard->Initialize();
		//読み込みしてないなら読み込みも行う
		if (TextureManager::GetTexture(data.textureName) == nullptr)
		{
			TextureManager::Load(data.textureName, data.textureName);
		}

		signboard->SetPicture(data.textureName);
		signboard->SetOutLineState({ 0,0,0,1.0f }, 0.1f);


		return true;
	}
	return false;
}

bool StageChanger::SetGoal(const LevelData::ObjectData& data)
{
	if (data.setObjectName == "goal")
	{
		SetObject<Goal>(data);
		
		//当たり判定を作成
		if (data.collider.have)
		{
			CollisionSet(data);
		}

		return true;
	}

	return false;
}

bool StageChanger::SetBossArea(const LevelData::ObjectData& data)
{
	if (data.setObjectName == "bossArea")
	{
		SetObject<BossArea>(data);
		
		BossArea* bArea = static_cast<BossArea*>(mEntitys.back().get());

		bArea->Create();

		return true;
	}
	return false;
}

bool StageChanger::SetRedCoin(const LevelData::ObjectData& data)
{
	if (data.setObjectName == "redcoin")
	{
		SetObject<RedCoin>(data);

		RedCoin* redcoin = static_cast<RedCoin*>(mEntitys.back().get());

		redcoin->Initialize();

		//当たり判定を作成
		if (data.collider.have)
		{
			CollisionSet(data);
		}

		if (data.eventtrigerName != "") {
			redcoin->mActive = false;
		}

		return true;
	}
	return false;
}

bool StageChanger::SetBreakBlock(const LevelData::ObjectData& data)
{
	if (data.setObjectName == "breakBlock")
	{
		SetObject<BreakBlock>(data);

		//当たり判定を作成
		if (data.collider.have)
		{
			CollisionSet(data);
		}

		return true;
	}
	return false;
}

bool StageChanger::SetDropBlock(const LevelData::ObjectData& data)
{
	if (data.setObjectName == "DropBlock")
	{
		SetObject<DropBlock>(data);

		//当たり判定を作成
		if (data.collider.have)
		{
			CollisionSet(data);
		}

		return true;
	}
	return false;
}

bool StageChanger::SetTikuwaBlock(const LevelData::ObjectData& data)
{
	if (data.setObjectName == "TikuwaBlock")
	{
		SetObject<TikuwaBlock>(data);

		//当たり判定を作成
		if (data.collider.have)
		{
			CollisionSet(data);
		}

		TikuwaBlock* tikuwa = static_cast<TikuwaBlock*>(mEntitys.back().get());

		tikuwa->saveScale = tikuwa->scale;
		tikuwa->saveBoxScale = tikuwa->box.scale;
		tikuwa->savePosition = tikuwa->position;

		return true;
	}
	return false;
}

void StageChanger::DrawModel()
{
	if (mShowModel == false) return;
	seaObject->Draw();
	for (auto& obj : mEntitys)
	{
		if (!obj->mActive)continue;
		//コリジョン用に配置したオブジェクトならスキップ
		if (obj->CheckTag(TagTable::NoDraw))continue;
		
		if (obj->CheckTag(TagTable::DitherTransparent))
		{
			BasicObjectPreDraw("DitherOutline",false);
		}
		else
		{
			BasicObjectPreDraw("OutLine", false);
			//BasicObjectPreDraw("DitherOutline", false);
		}
		obj->DrawOutLine();
		if (obj->CheckTag(TagTable::DitherTransparent))
		{
			BasicObjectPreDraw("DitherTransparent");
		}
		else
		{
			BasicObjectPreDraw("GroundToon");
			//BasicObjectPreDraw("DitherTransparent");
		}
		obj->Draw();
	}

	Player::Get()->Draw();
}

void StageChanger::DrawCollider()
{
	if (mShowCollider == false) return;
	for (auto& obj : CollideManager::Get()->allCols)
	{
		if (!obj->mActive)continue;
		if (!obj->CheckTag(TagTable::Collsion))continue;

		BasicObjectPreDraw("WireFrame");
		obj->box.Draw();
	}
	Player::Get()->DrawCollider();
}

void StageChanger::EventNameUniquePush(const std::string& eventname)
{
	//すでに同じものが入っているなら飛ばす
	for (auto& name : eventCameraNames)
	{
		if (name == eventname) {
			return;
		}
	}

	//そうでないなら入れる
	eventCameraNames.push_back(eventname);
}

void StageChanger::RegisterEvent(const std::string& eventname)
{
	if(Util::CheckString(eventname, "tutorialUI_"))
	{
		EventManager::Get()->Register<SummonUI>(eventname);
	}
	if (eventname == "stageTitleUI_mountain")
	{
		EventManager::Get()->Register<StageTitleUIMountain>(eventname);
	}
	if (eventname == "boss_hikiCamera") {
		EventManager::Get()->Register<BossHikiCamera>(eventname);
	}
	if (eventname == "getStarEvent_A") {
		EventManager::Get()->Register<GetStar>(eventname);
	}
	if (eventname == "redCoinEvent_A") {
		EventManager::Get()->Register<RedCoinEvent>(eventname);
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
