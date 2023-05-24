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
#include "DirectXInit.h"
#include <map>

const size_t kMaxSRVCount = 2056;

class Texture
{
public:

	void Load(const wchar_t* t);

	void CreateWhiteTexture();

	//�O������Q�Ƃ���p�̃��\�[�X�ݒ�(���������Ă��e�N�X�`�����ɂ͉e�����Ȃ�)
	D3D12_RESOURCE_DESC getResDesc;

	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff;
	
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = { 0 };

	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = { 0 };

private:
	TexMetadata metadata{};
	ScratchImage scratchImg{};
	ScratchImage mipChain{};

	//�V�F�[�_���\�[�X�r���[�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};//�ݒ�\����
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};

	DirectX12* dx12 = DirectX12::GetInstance();

};

class TextureManager
{
public:
	static TextureManager* Get() {
		static TextureManager instance;
		return &instance;
	};

	//�f�X�N���v�^�q�[�v�̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};

	ComPtr<ID3D12DescriptorHeap> srvHeap;

	//SRV�q�[�v�̑傫��
	UINT SRVHandleSize = 0;

	void Initialize();
	void PreLoad();

	static void Load(const std::string filepath,const std::string handle);

	static Texture* GetTexture(const std::string handle);

public:
	Texture white;
	/*Texture slime;
	Texture pizza;
	Texture def;
	Texture particle;
	Texture goalS;
	Texture blockTex;
	Texture wispTex;
	Texture titleTex;
	Texture startTex;
	Texture spaceTex;
	Texture goalBlockTex;
	Texture dashIcon;
	Texture dashCool;
	Texture hpBar;
	Texture redScreen;
	Texture setumei;
	Texture setumei2;
	Texture gameOver;*/

private:
	Texture bugfix;

	static std::map<std::string, Texture> textures;

	TextureManager(){};
	~TextureManager(){};
	TextureManager(const TextureManager& a) = delete;
	TextureManager operator=(const TextureManager&) = delete;

};