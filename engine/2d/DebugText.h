#pragma once
#include "Sprite.h"

class DebugText
{
public:
	void Initialize(const Texture& tex);
	void Print(const SpriteCommon& spritecommon, const std::string& text, float x,float y,float scale = 1.0f);

	void DrawAll();

	void PostDraw();

private:

	static const int32_t sMAX_CHAR_COUNT = 256;
	static const int32_t sFONT_WIDTH = 9;
	static const int32_t sFONT_HEIGHT = 18;
	static const int32_t sFONT_LINE_COUNT = 14;

	Sprite mSprites[sMAX_CHAR_COUNT];

	int32_t mSpriteIndex = 0;
};

