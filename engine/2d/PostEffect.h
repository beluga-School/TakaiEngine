#pragma once
#include "Sprite.h"

class PostEffect : public Sprite
{
public:
	PostEffect();

	void Initialize();

	void Draw();

	void PreDrawScene();

	void PostDrawScene();

private:
	//��ʃN���A�p�̐F
	static const float sClearColor[4];

	//�e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> mTexBuff;
	//SRV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> mDescHeapSRV;
	//�[�x�o�b�t�@
	ComPtr<ID3D12Resource> mDepthBuff;
	//RTV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> mDescHeapRTV;
	//DSV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> mDescHeapDSV;

	//�܂Ƃߊ֐�
	//�e�N�X�`������(���̉�ʂ���e�N�X�`���𐶐��H)
	void CreateTexture();

	//SRV(�V�F�[�_�[���\�[�X�r���[)�쐬
	void CreateSRV();

	//RTV(�����_�[�^�[�Q�b�g�r���[)�쐬
	void CreateRTV();
	
	//�[�x�o�b�t�@����
	void CreateDepthBuff();

	//DSV(�f�v�X�X�e���V���r���[)�쐬
	void CreateDSV();
};