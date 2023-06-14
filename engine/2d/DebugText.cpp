#include "DebugText.h"	

void DebugText::Initialize(const Texture& tex)
{
	for (int32_t i = 0; i < _countof(sprites); i++)
	{
		sprites[i].SetTexture(tex);
	}
}

void DebugText::Print(const SpriteCommon& spritecommon, const std::string& text,
	const float& x,const float& y,const float& scale)
{
	//�S�Ă̕����ɂ���
	for (int32_t i = 0; i < text.size(); i++)
	{
		//�ő啶��������
		if (spriteIndex >= maxCharCount) {
			break;
		}

		//1�������o��(*ASCII�R�[�h�ł������藧���Ȃ�)
		const unsigned char& character = text[i];

		//ASCII�R�[�h��2�i����΂����ԍ����v�Z
		int32_t fontIndex = character - 32;
		if (character >= 0x7f) {
			fontIndex = 0;
		}

		int32_t fontIndexY = fontIndex / fontLineCount;
		int32_t fontIndexX = fontIndex % fontLineCount;

		//���W�v�Z
		sprites[spriteIndex].mPosition = { x + fontWidth * scale * i,y,0 };
		sprites[spriteIndex].mTexLeftTop = 
		{(float)fontIndexX * fontWidth,(float)fontIndexY * fontHeight};
		sprites[spriteIndex].mCutSize = { fontWidth,fontHeight };
		sprites[spriteIndex].mSize = { fontWidth * scale,fontHeight * scale };

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
		sprites[i].mPosition = { 0,0,0 };
		sprites[i].mSize = { 0,0 };
	}
	spriteIndex = 0;
}
