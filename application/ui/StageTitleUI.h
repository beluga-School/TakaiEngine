#pragma once
#include "Sprite.h"
#include "TEasing.h"

/*! StageTitleUI
	ステージのタイトルを表示するUI
*/
class StageTitleUI
{
public:
	static void LoadResource();

	void ChangeHandle(int32_t stagenum);
	void Start();
	bool GetEnd();

	void Initialize();
	void Update();
	void Draw();

private:
	enum class State
	{
		None,
		DisPlay,
		Vanish,
	}state = State::None;
	
	Sprite stageTitle;

	std::vector<std::string> stageHandles;
	
	TEasing::easeTimer alphaTimer = 0.25f;
	TEasing::easeTimer disPlayTimer = 1.0f;

};

