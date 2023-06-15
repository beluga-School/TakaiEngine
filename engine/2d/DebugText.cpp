#include "DebugText.h"	

void DebugText::Initialize(const Texture& tex)
{
	for (int32_t i = 0; i < _countof(mSprites); i++)
	{
		mSprites[i].SetTexture(tex);
	}
}

void DebugText::Print(const SpriteCommon& spritecommon, const std::string& text,
	const float& x,const float& y,const float& scale)
{
	//�S�Ă̕����ɂ���
	for (int32_t i = 0; i < text.size(); i++)
	{
		//�ő啶��������
		if (mSpriteIndex >= sMAX_CHAR_COUNT) {
			break;
		}

		//1�������o��(*ASCII�R�[�h�ł������藧���Ȃ�)
		const unsigned char& character = text[i];

		//ASCII�R�[�h��2�i����΂����ԍ����v�Z
		int32_t fontIndex = character - 32;
		if (character >= 0x7f) {
			fontIndex = 0;
		}

		int32_t fontIndexY = fontIndex / sFONT_LINE_COUNT;
		int32_t fontIndexX = fontIndex % sFONT_LINE_COUNT;

		//���W�v�Z
		mSprites[mSpriteIndex].mPosition = { x + sFONT_WIDTH * scale * i,y,0 };
		mSprites[mSpriteIndex].mTexLeftTop = 
		{(float)fontIndexX * sFONT_WIDTH,(float)fontIndexY * sFONT_HEIGHT};
		mSprites[mSpriteIndex].mCutSize = { sFONT_WIDTH,sFONT_HEIGHT };
		mSprites[mSpriteIndex].mSize = { sFONT_WIDTH * scale,sFONT_HEIGHT * scale };

		SpriteTransferVertexBuffer(mSprites[mSpriteIndex]);

		mSprites[mSpriteIndex].Update();//SpriteUpdate(sprites[spriteIndex], spritecommon);

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
