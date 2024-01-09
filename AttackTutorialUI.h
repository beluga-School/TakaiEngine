#pragma once
#include "InterfaceUI.h"
#include "TEasing.h"
#include "TutorialUI.h"

class AttackTutorialUI final : public TutorialUI
{
public:
	static void LoadResource();
	void Update();
	void Draw();

	void Move(UIMove uimove);

private:
	//スライムのアニメーション
	Vector2 slimePos = { Util::WIN_WIDTH / 2,500 };
	Vector2 slimeSize = { 1.0f,1.0f };

	Color slimeColor = { 1,1,1,1 };

	TEasing::easeTimer standbyTimer = 1.0f;
	TEasing::easeTimer standbyTimer2 = 1.0f;

	TEasing::easeTimer attackTimer = 1.0f;
	TEasing::easeTimer slimeAlphaT = 1.0f;

	const float YSTART = 500.0f;
	const float YEND = 520.0f;

	//文字
	Vector2 stringPos = { Util::WIN_WIDTH / 2,580 };
	Color stringColor = { 1,1,1,1 };
};

