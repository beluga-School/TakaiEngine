#pragma once
#include "Sprite.h"
#include "Color.h"
#include <list>
#include <Billboard.h>

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

	static void DrawGraph(
		const float& x, const float& y,
		float sizerateX,float sizerateY,
		const std::string& handle, const Anchor& anchor = Anchor::CENTER);
	static void DrawGraph3D(const Vector3& pos,float width,float height,const std::string& handle);

	static void AllUpdate();
	static void AllDraw2D();
	static void AllDraw3D();

private:
	static std::list<Sprite> sSprites;

	static std::list<Billboard> sBillboards;
};