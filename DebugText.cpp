#include "DebugText.h"

void DebugText::Initialize(DirectX12 &dx12,Texture* tex)
{
	for (int i = 0; i < _countof(sprites); i++)
	{
		sprites[i] = SpriteCreate(dx12, tex,{0.5f,0.5f});
	}
}

void DebugText::Print(const SpriteCommon& spritecommon, const std::string& text, float x, float y, float scale)
{
	//�S�Ă̕����ɂ���
	for (int i = 0; i < text.size(); i++)
	{
		//�ő啶��������
		if (spriteIndex >= maxCharCount) {
			break;
		}

		//1�������o��(*ASCII�R�[�h�ł������藧���Ȃ�)
		const unsigned char& character = text[i];

		//ASCII�R�[�h��2�i����΂����ԍ����v�Z
		int fontIndex = character - 32;
		if (character >= 0x7f) {
			fontIndex = 0;
		}

		int fontIndexY = fontIndex / fontLineCount;
		int fontIndexX = fontIndex % fontLineCount;

		//���W�v�Z
		sprites[spriteIndex].position = { x + fontWidth * scale * i,y,0 };
		sprites[spriteIndex].texLeftTop = 
		{(float)fontIndexX * fontWidth,(float)fontIndexY * fontHeight};
		sprites[spriteIndex].texSize = { fontWidth,fontHeight };
		sprites[spriteIndex].size = { fontWidth * scale,fontHeight * scale };

		SpriteTransferVertexBuffer(sprites[spriteIndex]);

		SpriteUpdate(sprites[spriteIndex], spritecommon);

		spriteIndex++;
	}
}

void DebugText::DrawAll(DirectX12 &dx12, const SpriteCommon& spriteCommon)
{
	for (int i = 0; i < spriteIndex; i++)
	{
		SpriteDraw(sprites[i], dx12);
	}
}
