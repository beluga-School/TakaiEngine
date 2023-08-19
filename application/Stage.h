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

class StageChanger
{
public:
	void ChangeLevel(LevelData& data);

	//初期ステージを決定
	void Initialize(LevelData& data);
	void Update();
	void Draw();

	void DrawSprite();

	void Reload();

	std::string GetNowStageHandle();

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

	DokanInfo oldDokanInfo;

	//取得状況を初期化する
	void ResetRevise(int32_t stageNumber, int32_t starID,int32_t starnum);
	//取得状況を取得状態にする
	void CorrectedRevise(int32_t stageNumber, int32_t starID,int32_t starnum);
	//取得状態を読み取って、boolで返す
	bool LoadStarCorrect(int32_t stageNumber, int32_t starID);

private:
	StageChanger(){};
	~StageChanger(){};

	//ステージリロードの際に初期化するやつら
	void Reset();

	//通常のオブジェクト配置
	void NormalObjectSet(const LevelData::ObjectData& data);

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

	std::string currentHandle = "";

	LevelData* currentData = nullptr;

	GUI hoge = { "hoge" };
};

