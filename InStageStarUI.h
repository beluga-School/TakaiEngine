#pragma once
#include "UI.h"
#include <vector>

class InStageStarUI : public UI
{
public:
	InStageStarUI(const Vector2& pos, float scale) : UI(pos, scale)
	{

	}

	void LoadResource();

	void ChangeTexture(bool hoge, int32_t indent);

	void Initialize();
	void Update();
	void Draw();

	static InStageStarUI* Get()
	{
		static InStageStarUI instance({500,75},1);
		return &instance;
	}

private:
	Sprite stars[3];
};

