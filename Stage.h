#pragma once
#include "LevelLoader.h"
#include "Obj.h"
#include "Collision.h"
#include "EventBlock.h"
#include "GoalSystem.h"
#include "Goal.h"

class ColObj3d : public Obj3d
{
public:
	Obj3d* collideObj = nullptr;
};

class ColEventObj : public Obj3d
{
public:
	EventBlock* collideObj = nullptr;
};

class Stage
{
public:
	void ChangeLevel(LevelData& data);

	void Update();
	void Draw();

	void DrawSprite();

	std::string GetNowStageHandle();

	static Stage* Get()
	{
		static Stage instance;
		return &instance;
	}

	//モデルの配列
	std::list<Obj3d> mObj3ds;

	//当たり判定モデル配列
	std::list<ColObj3d> mColObj3ds;

	//当たり判定配列
	std::list<Cube> mColCubes;

	//イベントオブジェクト配列
	std::list<std::unique_ptr<EventBlock>> mEventObjects;

	std::list<ColEventObj> mColEventObjs;

	//ゴールオブジェクト配列
	std::list<std::unique_ptr<Goal>> mGoals;

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

