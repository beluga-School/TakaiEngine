#pragma once
#include "LevelLoader.h"
#include "Obj.h"
#include "Collision.h"
#include "EventBlock.h"
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

class ColEventObj : public Obj3d
{
public:
	EventBlock* collideObj = nullptr;
};

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

//イベントカメラデータをまとめて読み込む用のデータ配列
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

	//モデルの配列
	//Entityのポインタで保存した方が便利に使えるかもしれない
	std::list<Entity> mEntitys;

	//イベントオブジェクト配列
	std::list<std::unique_ptr<EventBlock>> mEventObjects;

	//ゴールオブジェクト配列
	std::list<std::unique_ptr<Goal>> mGoals;

	//大砲の制御点を一時的に保存する用配列
	std::vector<CannonPoint> mCannonPoints;

	//移動床の最終地点を一時的に保存する配列
	std::vector<MoveBlockPoint> mMoveBlockEndPoints;

	//スターを一時的に保存する変数
	std::vector<Star*> mTempStarSaves;

	//海オブジェクトを配置(ここより下の座標に落ちたらMISS判定)
	std::unique_ptr<Sea> seaObject;

	//コライダーを描画するか
	bool mShowCollider = false;

	//モデルを描画するか(コライダー描画時に邪魔になるので)
	bool mShowModel = true;

	GoalSystem goalSystem;

	DokanInfo saveNextDokanInfo;

	//取得状況を初期化する
	void ResetRevise(int32_t stageNumber, int32_t starID,int32_t starnum);
	//取得状況を取得状態にする
	void CorrectedRevise(int32_t stageNumber, int32_t starID,int32_t starnum);
	//取得状態を読み取って、boolで返す
	bool LoadStarCorrect(int32_t stageNumber, int32_t starID);

	//通常のオブジェクト配置
	void NormalObjectSet(const LevelData::ObjectData& data);

private:
	StageChanger(){};
	~StageChanger(){};

	//ステージリロードの際に初期化するやつら
	void Reset();

	//当たり判定配置
	void CollisionSet(const LevelData::ObjectData& data);

	//イベントオブジェクト用当たり判定配置(今後mEntityで一括管理したい)
	void CollisionSetEvent(const LevelData::ObjectData& data);

	//イベントオブジェクト(ブロック置き直し)
	void EvenyObjectSet(const LevelData::ObjectData& data);

	//ステージ切り替えの更新
	void ChangeUpdate();

	void SetPlayer(const LevelData::ObjectData& data);

	void DrawModel();
	void DrawCollider();

	void EventNameUniquePush(const std::string& eventname);

	std::string currentHandle = "";

	LevelData* currentData = nullptr;

	PlayerData playerData;

	std::unordered_map<std::string, std::vector<LoadCamData>> loadCamDatas;
	std::vector<LoadTargetData> loadTargetDatas;
	std::list<std::string> eventCameraNames;

	GUI hoge = { "hoge" };

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

			//イベント名、番号、カメラ位置を保存
			loadCamDatas[eventName].emplace_back(number, camdata);
		}
	}
};

