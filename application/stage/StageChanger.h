#pragma once
#include "LevelLoader.h"
#include "Obj.h"
#include "Collision.h"
#include "Entity.h"
#include "GoalSystem.h"
#include "Goal.h"
#include "Star.h"
#include "CollideManager.h"
#include "Mob.h"
#include "Dokan.h"
#include "Sea.h"
#include "EventCamera.h"
#include <vector>
#include "EventManager.h"

struct CannonPoint
{
	std::string key = "";
	Vector3 points{};
};

struct MoveBlockPoint
{
	std::string key = "";
	Vector3 points{};
};

struct PlayerData
{
	LevelData::ObjectData data;
	//土管での配置が見つかっているならそっちを優先するようにするフラグ
	bool dokanPriority = false;
};

enum CameraNumber
{
	Error = -1,
	Target = -2,
};

/*! LoadCamData
	イベントカメラデータをまとめて読み込む用の構造体
*/
struct LoadCamData
{
	LoadCamData(int32_t eventnumber_, EventCamData camData_) 
	{
		eventnumber = eventnumber_;
		camData.pos = camData_.pos;
		camData.rotation = camData_.rotation;
	};

	int32_t eventnumber = -1;
	
	EventCamData camData{};
};

/*! LoadTargetData
	イベントカメラのターゲット情報を一時保存するための構造体
*/
struct LoadTargetData
{
	LoadTargetData(std::string& eventname_,const Vector3& target_)
	{
		eventname = eventname_;
		target = target_;
	}

	std::string eventname = "";

	Vector3 target = {};
};

/*! StageChanger
	ステージを読み込み生成するクラス
*/
class StageChanger
{
public:
	static void LoadResource();

	void ChangeLevel(LevelData& data);

	//初期ステージを決定
	void Initialize(LevelData& data);
	void Update();
	void Draw();

	void DrawSprite();

	void Reload();

	std::string GetNowStageHandle();

	//現在読み込まれているステージのデータを返す
	LevelData* GetNowStageData();

	static StageChanger* Get()
	{
		static StageChanger instance;
		return &instance;
	}

	//取得状況を初期化する
	void ResetRevise(int32_t stageNumber, int32_t starID, int32_t starnum);
	//取得状況を取得状態にする
	void CorrectedRevise(int32_t stageNumber, int32_t starID, int32_t starnum);
	//取得状態を読み取って、boolで返す
	bool LoadStarCorrect(int32_t stageNumber, int32_t starID);

	//通常のオブジェクト配置
	bool SetBlock(const LevelData::ObjectData& data);

	//イベントオブジェクト配置
	template <class T> void SetObject(const LevelData::ObjectData& data)
	{
		mEntitys.emplace_back();
		mEntitys.back() = std::make_unique<T>();
		mEntitys.back()->Initialize();

		//コリジョン目的で配置したならオブジェクト配置を行わない
		if (data.fileName == "collision")
		{
			mEntitys.back()->SetTag(TagTable::NoDraw);
			return;
		}

		//アウトライン設定
		mEntitys.back()->SetOutLineState({ 0,0,0,1.0f }, 0.05f);

		//モデル設定
		//ファイルネームが設定されてるならそれで
		if (data.fileName != "")
		{
			//読み込みしてないなら読み込みも行う
			if (ModelManager::GetModel(data.fileName) == nullptr)
			{
				ModelManager::LoadModel(data.fileName, data.fileName, true);
			}
			mEntitys.back()->SetModel(ModelManager::GetModel(data.fileName));
		}

		//バグらないように白テクスチャを入れる
		mEntitys.back()->SetTexture(TextureManager::Get()->GetTexture("white"));

		if (data.textureName != "")
		{
			//指定テクスチャがあるならそっちを使う
			if (TextureManager::GetTexture(data.textureName) == nullptr)
			{
				std::string loadstr = "Resources\\" + data.textureName + ".png";
				TextureManager::Load(loadstr, data.textureName);
			}
			mEntitys.back()->SetTexture(TextureManager::Get()->GetTexture(data.textureName));
			mEntitys.back()->isTexDraw = true;
		}

		//タイリングの設定
		mEntitys.back()->mTiling = data.tiling;
		//イベント名を保持
		mEntitys.back()->eventName_ = data.eventtrigerName;
		//オブジェクトの配置
		LevelDataExchanger::SetObjectData(*mEntitys.back(), data);
	}

	//当たり判定配置
	void CollisionSet(const LevelData::ObjectData& data);

	void EnemyCollisionSet(const LevelData::ObjectData& data);

private:
	StageChanger(){};
	~StageChanger(){};

	//ステージリロードの際に初期化するやつら
	void Reset();

	//ステージ切り替えの更新
	void ChangeUpdate();

	//内部で指定したイベント名が入ったら対応したクラスをイベント登録する
	void RegisterEvent(const std::string& eventname);

	//オブジェクトごとの設置処理
	//プレイヤー
	void SetPlayer(const LevelData::ObjectData& data);
	//土管
	bool SetDokan(const LevelData::ObjectData& data);
	//敵土管
	bool SetEnemyDokan(const LevelData::ObjectData& data);
	//星
	bool SetStar(const LevelData::ObjectData& data);
	//大砲
	bool SetCannon(const LevelData::ObjectData& data);
	//移動ブロック
	bool SetMoveBlock(const LevelData::ObjectData& data);
	//イベントトリガー
	bool SetEventTrigger(const LevelData::ObjectData& data);
	//看板
	bool SetSignBoard(const LevelData::ObjectData& data);
	//ゴール
	bool SetGoal(const LevelData::ObjectData& data);
	//ボスエリア
	bool SetBossArea(const LevelData::ObjectData& data);
	//赤コインの配置
	bool SetRedCoin(const LevelData::ObjectData& data);
	//破壊可能ブロックの配置
	bool SetBreakBlock(const LevelData::ObjectData& data);
	//落ちるブロックの処理
	bool SetDropBlock(const LevelData::ObjectData& data);
	//プレイヤーが乗ったら落ちるブロックの実装
	bool SetTikuwaBlock(const LevelData::ObjectData& data);
	//動く草
	bool SetGrass(const LevelData::ObjectData& data);

	void DrawModel();
	void DrawCollider();

	void EventNameUniquePush(const std::string& eventname);

	template <class TEventCamera> void CameraLoader(const LevelData::ObjectData& data, const std::string& eventname)
	{
		EventCamData camdata;
		camdata.pos = data.translation;
		camdata.rotation = data.rotation;

		if (Util::CheckString(data.eventtrigerName, eventname))
		{
			//数字を分離
			int32_t number = Util::GetNumber(data.eventtrigerName, "_");
			//イベント名を分離
			std::string eventName = Util::GetString(data.eventtrigerName, eventname);

			EventManager::Get()->Register<TEventCamera>(eventName);

			EventNameUniquePush(eventName);

			//ターゲットであるなら
			if (Util::CheckString(data.eventtrigerName, "target"))
			{
				//ターゲットを示す番号を挿入
				loadTargetDatas.emplace_back(eventName, camdata.pos);
				//残りを飛ばす
				return;
			}

			if (data.textureName != "")
			{

			}

			//イベント名、番号、カメラ位置を保存
			loadCamDatas[eventName].emplace_back(number, camdata);
		}
	}

public:
	//モデルの配列
	std::list<std::unique_ptr<Entity>> mEntitys;

	//大砲の制御点を一時的に保存する配列
	std::vector<CannonPoint> mCannonPoints;

	//移動床の最終地点を一時的に保存する配列
	std::vector<MoveBlockPoint> mMoveBlockEndPoints;

	//海オブジェクトを配置(ここより下の座標に落ちたらMISS判定)
	std::unique_ptr<Sea> seaObject;

	//コライダーを描画するか
	bool mShowCollider = false;

	//モデルを描画するか(コライダー描画時に邪魔になるので)
	bool mShowModel = true;

	GoalSystem goalSystem;

	DokanInfo saveNextDokanInfo;

	PlayerData playerData;
private:
	std::string currentHandle = "";

	LevelData* currentData = nullptr;

	std::unordered_map<std::string, std::vector<LoadCamData>> loadCamDatas;
	std::vector<LoadTargetData> loadTargetDatas;
	std::list<std::string> eventCameraNames;

	GUI mRotCheck = { "hoge" };

};

