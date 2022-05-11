#pragma once
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")
#include <DirectXTex.h>

//自分でクラス化したやつ
#include "WinAPI.h"
#include "Result.h"
#include "Input.h"
#include "DirectXInit.h"
#include "Vertex.h"
#include "Shader.h"

struct ConstBufferDataMaterial {
	XMFLOAT4 color;	//色(RGBA)
};

struct ConstBufferDataTransform {
	XMMATRIX mat;	//3D変換行列
};

class ConstBuffer
{
	ID3D12Resource* CreateConstantBuff(
		D3D12_HEAP_PROPERTIES cbHeapProp,
		D3D12_RESOURCE_DESC cbResourceDesc,
		DirectXInit dx12_,
		ID3D12Resource* countBuff,
		ConstBufferDataMaterial* constMap);

	ID3D12Resource* CreateConstantBuff(
		D3D12_HEAP_PROPERTIES cbHeapProp,
		D3D12_RESOURCE_DESC cbResourceDesc,
		DirectXInit dx12_,
		ID3D12Resource* countBuff,
		ConstBufferDataTransform* constMap);
};

