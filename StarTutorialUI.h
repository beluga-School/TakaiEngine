#pragma once
#include "TEasing.h"
#include "InterfaceUI.h"
#include "Util.h"
#include "Color.h"
#include "ImguiManager.h"

class StarTutorialUI
{
public:
	static void LoadResource();
	void Update();
	void Draw();

	void Appearance();
	void Leave();

	void Move(UIMove uimove);

	void Gui();

private:
	//文字
	Vector2 stringpos = { Util::WIN_WIDTH / 2, 580};
	Color stringColor = {1,1,1,1};
	//背景
	Vector2 backpos = { Util::WIN_WIDTH / 2, 500 };
	Color backColor = {0.1f,0.1f,0.1f,0.5f};
	//出現消滅のタイマー
	TEasing::easeTimer alphaTimer = 0.5f;
	//星
	Vector2 starpos = { Util::WIN_WIDTH / 2, 580 };
	float starAngle = 0;
	TEasing::easeTimer sAngleTimer = 2.0f;
	TEasing::easeTimer sStandbyTimer = 1.0f;
	TEasing::easeTimer sSpawnT = 1.0f;
	Color starColor = { 1.0f,1.0f,1.0f,1.0f };
	//星の背景
	TEasing::easeTimer sBackAlphaT = 1.0f;
	TEasing::easeTimer sStandbyTimer2 = 1.0f;
	Color starBackC = { 1.0f,1.0f,1.0f,1.0f };

	GUI tutorialUIgui = { "t" };
};

