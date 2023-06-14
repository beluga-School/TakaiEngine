#pragma once
#include "Sprite.h"

class DebugText
{
public:
	void Initialize(const Texture& tex);
	void Print(const SpriteCommon& spritecommon, const std::string& text,
		const float& x,const float& y,const float& scale = 1.0f);

	void DrawAll();

	void PostDraw();

private:

	static const int32_t maxCharCount = 256;
	static const int32_t fontWidth = 9;
	static const int32_t fontHeight = 18;
	static const int32_t fontLineCount = 14;

	Sprite sprites[maxCharCount];

	int32_t spriteIndex = 0;
};

