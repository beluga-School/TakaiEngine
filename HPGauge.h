#pragma once
#include "Sprite.h"
#include "Float4.h"

class HPGauge
{
public:

	void Initialize();
	void Update();
	void Draw();

	void ColorChange(Float4 color,int32_t index);

	bool mIsVisilve = true;

private:
	Vector2 pos;

	Sprite ui[8];
};

