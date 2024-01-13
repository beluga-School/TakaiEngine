#pragma once
#include "Util.h"
#include "TEasing.h"
#include "Color.h"

class TutorialUI
{
public:
	void Reset();
protected:
	//背景
	Vector2 backpos = { Util::WIN_WIDTH / 2, 500 };
	Color backColor = { 0.1f,0.1f,0.1f,0.5f };
	//出現消滅のタイマー
	TEasing::easeTimer alphaTimer = 0.5f;

	void BackUpdate();
	void BackDraw();
};

