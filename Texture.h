#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")
#include <DirectXTex.h>
#include "Model.h"

const size_t kMaxSRVCount = 2056;

struct TextureData
{
	D3D12_GPU_DESCRIPTOR_HANDLE handle;

	ID3D12DescriptorHeap* srvHeap;

	//�O������Q�Ƃ���p�̃��\�[�X�ݒ�(���������Ă��e�N�X�`�����ɂ͉e�����Ȃ�)
	D3D12_RESOURCE_DESC getResDesc;
};

class Texture
{
public:
	//�Ȃ����֐����ɓ������r�[DX12���S�ď��������ꂽ�̂ň����ł�����Ă���
	void Load(const wchar_t* t, DirectX12 DX12);

	void CreateWhiteTexture(DirectX12 DX12);

	TextureData texData;

	D3D12_GPU_DESCRIPTOR_HANDLE GetHandle();
	//�ݒ������SRV�p�f�X�N���v�^�q�[�v�𐶐�
	//������ComPtr�ɂ���ƃ_�������� �Ȃ�Ń_�����͂킩���
	//ID3D12DescriptorHeap* srvHeap;

	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff;
	
private:
	TexMetadata metadata{};
	ScratchImage scratchImg{};
	ScratchImage mipChain{};

	//D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = { 0 };

	//SRV�̍ő��
	
	//�f�X�N���v�^�q�[�v�̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	
	//SRV�q�[�v�̐擪�n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = { 0 };
	//�V�F�[�_���\�[�X�r���[�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};//�ݒ�\����
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};
	//SRV�q�[�v�̑傫��
	UINT SRVHandleSize = 0;

};

