#pragma once
#include "Sprite.h"
#include "Color.h"
#include <list>

class InstantDrawer
{
public:
	enum class Anchor
	{
		LEFT,
		CENTER,
		RIGHT,
	};

	static void DrawInit();

	static void DrawBox(
		const float& x, const float& y,
		const float& width, const float& height,
		const Color& color, const Anchor& anchor = Anchor::CENTER);

	static void AllUpdate();
	static void AllDraw();

private:
	static std::list<Sprite> sSprites;
};