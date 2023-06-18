#pragma once
#include "Sprite.h"

class PostEffect :public Sprite
{
public:
	PostEffect();

	void Initialize();

	void Draw();

private:
	//�e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> mTexBuff;
	//SRV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> mDescHeapSRV;
};