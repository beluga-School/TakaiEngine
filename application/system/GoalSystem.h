#pragma once
#include "Sprite.h"
#include "TEasing.h"

/*! ClearManage
	クリア状況の管理を行うクラス
*/
class ClearManage
{
public:
	static ClearManage* Get()
	{
		static ClearManage instance;
		return &instance;
	}

	void SetClearInfo(const std::string& clearStageName = "");

private:
	~ClearManage(){};
	ClearManage(){};
};

/*! Goal
	ゴール後の処理を行うクラス
*/
class GoalSystem
{
public:
	void LoadResource();

	void AnimetionStart();
	void GoalAnimetion();

	void Initialize();
	void Update();
	void Draw();

	//クリアフラグ
	bool mIsClear = false;

private:
	Sprite mClearSprite;

	enum class AnimetionState
	{
		None,
		First,
		Second,
		Third,
	} animetionState = AnimetionState::None;

	TEasing::easeTimer firstTimer = 1.0f;
	TEasing::easeTimer secondTimer = 0.5f;
	TEasing::easeTimer thirdTimer = 1.0f;

};

