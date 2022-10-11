#pragma once
#include "Sprite.h"

class DebugText
{
public:
	void Initialize(DirectX12 &dx12, Texture* tex);
	void Print(const SpriteCommon& spritecommon, const std::string& text, float x, float y, float scale);

	void DrawAll(DirectX12 &dx12, const SpriteCommon& spriteCommon);

private:

	static const int maxCharCount = 256;
	static const int fontWidth = 9;
	static const int fontHeight = 18;
	static const int fontLineCount = 14;

	Sprite sprites[maxCharCount];

	int spriteIndex = 0;
};

