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

class Stage
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

	static Stage* Get()
	{
		static Stage instance;
		return &instance;
	}

	//モデルの配列
	//Entityのポインタで保存した方が便利に使えるかもしれない
	//それはまた今度
	std::list<Entity> mEntitys;

	//イベントオブジェクト配列
	std::list<std::unique_ptr<EventBlock>> mEventObjects;

	//ゴールオブジェクト配列
	std::list<std::unique_ptr<Goal>> mGoals;

	//大砲の制御点を一時的に保存する用配列
	std::vector<CannonPoint> mCannonPoints;

	//コライダーを描画するか
	bool mShowCollider = false;

	//モデルを描画するか(コライダー描画時に邪魔になるので)
	bool mShowModel = true;

	GoalSystem goalSystem;

private:
	Stage(){};
	~Stage(){};

	//通常のオブジェクト配置
	void NormalObjectSet(const LevelData::ObjectData& data);

	//当たり判定配置
	void CollisionSet(const LevelData::ObjectData& data);

	//イベントオブジェクト(ブロック置き直し)
	void EvenyObjectSet(const LevelData::ObjectData& data);

	//ステージ切り替えの更新
	void ChangeUpdate();

	void DrawModel();
	void DrawCollider();

	std::string currentHandle = "";

	LevelData* currentData = nullptr;
};

