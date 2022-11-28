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

const size_t kMaxSRVCount = 2056;

class Texture
{
public:

	void Load(const wchar_t* t);

	void CreateWhiteTexture();

	//外部から参照する用のリソース設定(書き換えてもテクスチャ側には影響しない)
	D3D12_RESOURCE_DESC getResDesc;

	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff;
	
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = { 0 };

	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = { 0 };

private:
	TexMetadata metadata{};
	ScratchImage scratchImg{};
	ScratchImage mipChain{};

	//シェーダリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};//設定構造体
	//リソース設定
	D3D12_RESOURCE_DESC resDesc{};

	DirectX12* dx12 = DirectX12::GetInstance();

};

class TextureManager
{
public:
	static TextureManager* GetInstance() {
		static TextureManager instance;
		return &instance;
	};

	//デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};

	ComPtr<ID3D12DescriptorHeap> srvHeap;

	//SRVヒープの大きさ
	UINT SRVHandleSize = 0;

	void Initialize();
	void PreLoad();

public:
	Texture white;
	Texture slime;
	Texture pizza;
	Texture def;
	Texture particle;

private:
	TextureManager(){};
	~TextureManager(){};
	TextureManager(const TextureManager& a) = delete;
	TextureManager operator=(const TextureManager&) = delete;

};