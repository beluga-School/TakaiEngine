#include "DebugText.h"

void DebugText::Initialize(Texture* tex)
{
	for (int32_t i = 0; i < _countof(sprites); i++)
	{
		sprites[i].SetTexture(tex);
		//SpriteCreate(&sprites[i],tex,{0.5f,0.5f});
	}
}

void DebugText::Print(const SpriteCommon& spritecommon, const std::string& text, float x, float y, float scale)
{
	//全ての文字について
	for (int32_t i = 0; i < text.size(); i++)
	{
		//最大文字数超過
		if (spriteIndex >= maxCharCount) {
			break;
		}

		//1文字取り出す(*ASCIIコードでしか成り立たない)
		const unsigned char& character = text[i];

		//ASCIIコードの2段分飛ばした番号を計算
		int32_t fontIndex = character - 32;
		if (character >= 0x7f) {
			fontIndex = 0;
		}

		int32_t fontIndexY = fontIndex / fontLineCount;
		int32_t fontIndexX = fontIndex % fontLineCount;

		//座標計算
		sprites[spriteIndex].position = { x + fontWidth * scale * i,y,0 };
		sprites[spriteIndex].texLeftTop = 
		{(float)fontIndexX * fontWidth,(float)fontIndexY * fontHeight};
		sprites[spriteIndex].cutSize = { fontWidth,fontHeight };
		sprites[spriteIndex].size = { fontWidth * scale,fontHeight * scale };

		SpriteTransferVertexBuffer(sprites[spriteIndex]);

		sprites[spriteIndex].Update();//SpriteUpdate(sprites[spriteIndex], spritecommon);

		spriteIndex++;
	}
}

void DebugText::DrawAll()
{
	for (int32_t i = 0; i < spriteIndex; i++)
	{
		sprites[i].Draw();
	}
}

void DebugText::PostDraw()
{
	for (int32_t i = 0; i < spriteIndex; i++)
	{
		sprites[i].position = { 0,0,0 };
		sprites[i].size = { 0,0 };
	}
	spriteIndex = 0;
}
