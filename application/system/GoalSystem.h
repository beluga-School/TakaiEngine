#pragma once
#include "Sprite.h"
#include "TEasing.h"

class ClearManage
{
public:
	static ClearManage* Get()
	{
		static ClearManage instance;
		return &instance;
	}

	//ゴールしたらゴール演出を実行するための情報
	bool isClear = false;
	int32_t eventNumber = -1;

	void SetClearInfo(const std::string& stageName = "");

	//これでよくない？？？？？？？？？？？？？？？？？？？
	//ここが初めてtrueになったときに解放演出が入る
	bool stage1clear = 0;
	bool stage2clear = 0;
	bool stage3clear = 0;
	bool stage4clear = 0;
	bool stage5clear = 0;
	bool stage6clear = 0;

private:
	~ClearManage(){};
	ClearManage(){};
};

//ゴール後の処理を進めるクラス
//今はスプライト出すだけ
//今後ステージに持たせて、1ステージ=1ゴールをもつ、という形にしたい
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

