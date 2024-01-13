#pragma once
#include "TutorialUI.h"
#include "InterfaceUI.h"

class DokanTutorialUI : public TutorialUI
{
public:
	static void LoadResource();
	void Update();
	void Draw();

	void Move(UIMove uimove);
private:
	void Apparance();

	Vector2 dokanPos = { Util::WIN_WIDTH / 2,450 };
	Vector2 playerPos = { Util::WIN_WIDTH / 2,400 };
	Vector2 stringPos = { Util::WIN_WIDTH / 2,580 };

	Color color = {1.f,1.f,1.f,1.f};
	Color playerColor = {1.f,1.f,1.f,1.f};

	TEasing::easeTimer timer = 1.0f;
	TEasing::easeTimer standbyTimer = 1.0f;
	TEasing::easeTimer standbyTimer2 = 1.0f;
};

