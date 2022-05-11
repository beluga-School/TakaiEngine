#include "ConstBuffer.h"

ID3D12Resource* ConstBuffer::CreateConstantBuff(
	D3D12_HEAP_PROPERTIES cbHeapProp,
	D3D12_RESOURCE_DESC cbResourceDesc,
	DirectXInit dx12_,
	ID3D12Resource* countBuff,
	ConstBufferDataMaterial* constMap)
{
	//ヒープ設定
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPUへの転送用
	//リソース設定
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;	//256バイトアラインメント
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//定数バッファの生成
	result = dx12_.device->CreateCommittedResource(
		&cbHeapProp,	//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,	//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&countBuff));
	assert(SUCCEEDED(result));

	//定数バッファのマッピング
	result = countBuff->Map(0, nullptr, (void**)&constMap);	//マッピング
	assert(SUCCEEDED(result));

	return countBuff;
}

ID3D12Resource* ConstBuffer::CreateConstantBuff(
	D3D12_HEAP_PROPERTIES cbHeapProp,
	D3D12_RESOURCE_DESC cbResourceDesc,
	DirectXInit dx12_,
	ID3D12Resource* countBuff,
	ConstBufferDataTransform* constMap)
{
	//ヒープ設定
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPUへの転送用
	//リソース設定
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;	//256バイトアラインメント
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//定数バッファの生成
	result = dx12_.device->CreateCommittedResource(
		&cbHeapProp,	//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,	//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&countBuff));
	assert(SUCCEEDED(result));

	//定数バッファのマッピング
	result = countBuff->Map(0, nullptr, (void**)&constMap);	//マッピング
	assert(SUCCEEDED(result));

	return countBuff;
}