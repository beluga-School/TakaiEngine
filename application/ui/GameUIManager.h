#pragma once
#include <StarUI.h>
#include <StageTitleUI.h>

//ゲーム画面上に出てくる2DUIを管理するマネージャー
class GameUIManager
{
public:
	static GameUIManager* Get() {
		static GameUIManager instance;
		return &instance;
	};

	static void LoadResource();

	void Move(UIMove uimove);

	void Initialize();
	void Update();
	void Draw();

	StageTitleUI* GetStageTitleUI()
	{
		return &stageTitleUI;
	}

private:
	GameUIManager() {};
	~GameUIManager() {};

	StarUI starUI;
	StageTitleUI stageTitleUI;
};

