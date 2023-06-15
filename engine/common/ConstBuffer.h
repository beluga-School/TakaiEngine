#pragma once
#include <d3d12.h>
#include "DirectXInit.h"
#include "Result.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

#include "Matrix4.h"

struct ConstBufferDataMaterial {
	XMFLOAT4 color;	//色(RGBA)
};

struct ConstBufferDataSprite {
	XMFLOAT4 color;	//色(RGBA)
	XMMATRIX mat;	//3D変換行列
};

template <class Format>

class ConstBuffer
{
public:
	Microsoft::WRL::ComPtr<ID3D12Resource> mBuffer;
	Format* mConstBufferData;

	ConstBuffer();
	~ConstBuffer();
};

template<class Format>
inline ConstBuffer<Format>::ConstBuffer()
{
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(Format) + 0xff) & ~0xff;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//定数バッファの生成
	result = DirectX12::Get()->mDevice->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&mBuffer));
	assert(SUCCEEDED(result));

	result = mBuffer->Map(0, nullptr, (void**)&mConstBufferData);
	assert(SUCCEEDED(result));
}

template<class Format>
inline ConstBuffer<Format>::~ConstBuffer()
{
	mBuffer->Unmap(0, nullptr);
}
