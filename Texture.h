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

class Texture
{
public:
	//なぜか関数内に入った途端DX12が全て初期化されたので引数でもらっている
	void Load(const wchar_t* t, Model cube, DirectX12 DX12);
	D3D12_GPU_DESCRIPTOR_HANDLE GetHandle();
	//設定を元にSRV用デスクリプタヒープを生成
	ID3D12DescriptorHeap* srvHeap = nullptr;

private:
	TexMetadata metadata{};
	ScratchImage scratchImg{};
	ScratchImage mipChain{};

	ID3D12Resource* texBuff = nullptr;
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = { 0 };

	//SRVの最大個数
	const size_t kMaxSRVCount = 2056;
	//デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	
	//SRVヒープの先頭ハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = { 0 };
	//シェーダリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};//設定構造体
	//リソース設定
	D3D12_RESOURCE_DESC resDesc{};
	//SRVヒープの大きさ
	UINT SRVHandleSize = 0;

};

