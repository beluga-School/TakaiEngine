#pragma once
#include <d3d12.h>
#include "DirectXInit.h"
#include "Result.h"
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include "Matrix4.h"

struct ConstBufferDataMaterial {
	DirectX::XMFLOAT4 color;	//色(RGBA)
};

struct ConstBufferDataSprite {
	DirectX::XMFLOAT4 color;	//色(RGBA)
	DirectX::XMMATRIX mat;	//3D変換行列
};

template <class Format>

/*! ConstBuffer
	定数バッファ
*/
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
	sResult = DirectX12::Get()->mDevice->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&mBuffer));
	assert(SUCCEEDED(sResult));

	sResult = mBuffer->Map(0, nullptr, (void**)&mConstBufferData);
	assert(SUCCEEDED(sResult));
}

template<class Format>
inline ConstBuffer<Format>::~ConstBuffer()
{
	mBuffer->Unmap(0, nullptr);
}
