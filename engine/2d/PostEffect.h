#pragma once
#include "Sprite.h"

class PostEffect 
{
public:
	PostEffect();

	void Initialize();

	void Update();

	void Draw();

	void PreDrawScene();

	void PostDrawScene();

	//�p�C�v���C���؂�ւ��p�ɂƂ肠�����u���Ă���
	std::string pipeLineName = "None";

private:
	//��ʃN���A�p�̐F
	static const float sClearColor[4];

	//�e�N�X�`���o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> mTexBuff[2];
	//SRV�p�f�X�N���v�^�q�[�v
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mDescHeapSRV;
	//�[�x�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> mDepthBuff;
	//RTV�p�f�X�N���v�^�q�[�v
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mDescHeapRTV;
	//DSV�p�f�X�N���v�^�q�[�v
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mDescHeapDSV;

	//���_�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> mVertBuff;
	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW mVbView{};
	//�萔�o�b�t�@
	ConstBuffer<ConstBufferDataSprite> mConstBuffer;

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