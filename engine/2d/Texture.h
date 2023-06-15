#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

#pragma warning (push)
#pragma warning( disable : 26813 )
#include <DirectXTex.h>
#pragma warning (pop)

#include "DirectXInit.h"
#include <map>

const size_t kMaxSRVCount = 2056;

class Texture
{
public:

	void Load(const wchar_t& t);

	void CreateWhiteTexture();

	//�O������Q�Ƃ���p�̃��\�[�X�ݒ�(���������Ă��e�N�X�`�����ɂ͉e�����Ȃ�)
	D3D12_RESOURCE_DESC mGetResDesc;

	Microsoft::WRL::ComPtr<ID3D12Resource> mTexBuff;
	
	D3D12_GPU_DESCRIPTOR_HANDLE mGpuHandle = { 0 };

	D3D12_CPU_DESCRIPTOR_HANDLE mCpuHandle = { 0 };

private:
	TexMetadata mMetadata{};
	ScratchImage mScratchImg{};
	ScratchImage mMipChain{};

	//�V�F�[�_���\�[�X�r���[�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC mSrvDesc{};//�ݒ�\����
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC mResDesc{};

};

class TextureManager
{
public:
	static TextureManager* Get() {
		static TextureManager instance;
		return &instance;
	};

	//�f�X�N���v�^�q�[�v�̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC mSrvHeapDesc = {};

	ComPtr<ID3D12DescriptorHeap> mSrvHeap;

	//SRV�q�[�v�̑傫��
	UINT mSRVHandleSize = 0;

	void Initialize();
	void PreLoad();

	static void Load(const std::string &filepath,const std::string &handle);
	
	static Texture* GetTexture(const std::string &handle);


private:
	//�e�N�X�`����1���ڂ��A�f�o�b�O�e�L�X�g�̕��ɂȂ�������Ă�o�O������̂ł��������p�̃e�N�X�`��
	Texture bugfix_;

	static std::map<std::string, Texture> mTextures;

	TextureManager(){};
	~TextureManager(){};
	TextureManager(const TextureManager& a) = delete;
	TextureManager operator=(const TextureManager&) = delete;

};