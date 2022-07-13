#pragma once
#include <d3d12.h>
#include "DirectXInit.h"
#include "Model.h"
#include "Result.h"

struct ConstBufferDataMaterial {
	XMFLOAT4 color;	//êF(RGBA)
};

struct ConstBufferDataTransform {
	XMMATRIX mat;	//3Dïœä∑çsóÒ
};

template <class Format> class ConstBuffer
{
public:
	Microsoft::WRL::ComPtr<ID3D12Resource> buffer;
	Format* constBufferData;

	ConstBuffer();
	~ConstBuffer();
};

template<class Format> inline ConstBuffer<Format>::ConstBuffer()
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

	result = DX12.device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&buffer));
	assert(SUCCEEDED(result));

	result = buffer->Map(0, nullptr, (void**)&constBufferData);
	assert(SUCCEEDED(result));
}

template<class Format>
inline ConstBuffer<Format>::~ConstBuffer()
{
	buffer->Unmap(0, nullptr);
}
