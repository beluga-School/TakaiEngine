#pragma once
#include <StageTitleUI.h>
#include <UI.h>

/*! GameUIManager
	ゲーム画面上に出てくる2DUIを管理するマネージャー
*/
class GameUIManager
{
public:
	static GameUIManager* Get() {
		static GameUIManager instance;
		return &instance;
	};

	static void LoadResource();

	void Move(UIMove uimove,const std::string& handle);

	void Reset();

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

	StageTitleUI stageTitleUI;

	UI tutorialEyeMove;
	UI tutorialJump;
	UI tutorialMove;
	UI selectTitleUIMountain;
	UI tutorialEnemyDown;
};

