#pragma once
#include "Sprite.h"
#include "TEasing.h"

class StageTitleUI
{
public:
	static void LoadResource();

	void ChangeHandle(int32_t stagenum);
	void Start();

	void Initialize();
	void Update();
	void Draw();

	static StageTitleUI* Get();

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

