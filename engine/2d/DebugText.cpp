#include "DebugText.h"	

void DebugText::Initialize(const Texture& tex)
{
	for (int32_t i = 0; i < _countof(mSprites); i++)
	{
		mSprites[i].SetTexture(tex);
	}
}

void DebugText::Print(const std::string& text,float x,float y,float scale)
{
	//全ての文字について
	for (int32_t i = 0; i < text.size(); i++)
	{
		//最大文字数超過
		if (mSpriteIndex >= sMAX_CHAR_COUNT) {
			break;
		}

		//1文字取り出す(*ASCIIコードでしか成り立たない)
		const uint8_t& CHARACTER = text[i];

		//ASCIIコードの2段分飛ばした番号を計算
		int32_t fontIndex = CHARACTER - 32;
		if (CHARACTER >= 0x7f) {
			fontIndex = 0;
		}

		int32_t fontIndexY = fontIndex / sFONT_LINE_COUNT;
		int32_t fontIndexX = fontIndex % sFONT_LINE_COUNT;

		//座標計算
		mSprites[mSpriteIndex].mPosition = { x + sFONT_WIDTH * scale * i,y,0 };
		mSprites[mSpriteIndex].mTexLeftTop = 
		{(float)fontIndexX * sFONT_WIDTH,(float)fontIndexY * sFONT_HEIGHT};
		mSprites[mSpriteIndex].mCutSize = { sFONT_WIDTH,sFONT_HEIGHT };
		mSprites[mSpriteIndex].mSize = { sFONT_WIDTH * scale,sFONT_HEIGHT * scale };

		SpriteTransferVertexBuffer(mSprites[mSpriteIndex]);

		mSprites[mSpriteIndex].Update();
		mSpriteIndex++;
	}
}

void DebugText::DrawAll()
{
	for (int32_t i = 0; i < mSpriteIndex; i++)
	{
		mSprites[i].Draw();
	}
}

void DebugText::PostDraw()
{
	for (int32_t i = 0; i < mSpriteIndex; i++)
	{
		mSprites[i].mPosition = { 0,0,0 };
		mSprites[i].mSize = { 0,0 };
	}
	mSpriteIndex = 0;
}
