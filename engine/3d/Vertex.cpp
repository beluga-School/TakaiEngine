#include "Vertex.h"
#include "Result.h"

void VertexData::CreateVertex(const std::vector<Vertex> &vertices,const std::vector<uint16_t> &indices)
{
	//頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * vertices.size());

	//頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapProp{};		//ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPUへの転送用

	//リソース設定
	mResDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	mResDesc.Width = sizeVB;	//頂点データ全体のサイズ
	mResDesc.Height = 1;
	mResDesc.DepthOrArraySize = 1;
	mResDesc.MipLevels = 1;
	mResDesc.SampleDesc.Count = 1;
	mResDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//頂点バッファの生成
	//ComPtrにしたらダメだった マップ処理に使ってるから？
	result = DirectX12::Get()->mDevice->CreateCommittedResource(
		&heapProp,	//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&mResDesc,	//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(mVertBuff.GetAddressOf()));
	assert(SUCCEEDED(result));

	//GPU状のバッファに対応した仮想メモリ(メインメモリ上)を取得
	Vertex* vertMap = nullptr;
	result = mVertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	//全頂点に対して
	for (int32_t i = 0; i < vertices.size(); i++) {
		vertMap[i] = vertices[i];	//	座標をコピー
	}
	//繋がりを解除
	mVertBuff->Unmap(0, nullptr);

	//GPU仮想アドレス
	mVbView.BufferLocation = mVertBuff->GetGPUVirtualAddress();
	//頂点バッファのサイズ
	mVbView.SizeInBytes = sizeVB;
	//頂点1つ分のデータサイズ
	mVbView.StrideInBytes = sizeof(vertices[0]);

	//インデックスデータ全体のサイズ
	//ComPtrにしたらダメだった マップ処理に使ってるから？
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * indices.size());

	//リソース設定
	mResDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	mResDesc.Width = sizeIB;	//インデックス情報が入る分のサイズ
	mResDesc.Height = 1;
	mResDesc.DepthOrArraySize = 1;
	mResDesc.MipLevels = 1;
	mResDesc.SampleDesc.Count = 1;
	mResDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//インデックスバッファの生成
	result = DirectX12::Get()->mDevice->CreateCommittedResource(
		&heapProp,	//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&mResDesc,	//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(mIndexBuff.GetAddressOf()));

	//インデックスバッファをマッピング
	uint16_t* indexMap = nullptr;
	result = mIndexBuff->Map(0, nullptr, (void**)&indexMap);
	//全インデックスに対して
	for (int32_t i = 0; i < indices.size(); i++) {
		indexMap[i] = indices[i];	//インデックスをコピー
	}
	//マッピング解除
	mIndexBuff->Unmap(0, nullptr);

	//インデックスバッファビューの作成
	mIbView.BufferLocation = mIndexBuff->GetGPUVirtualAddress();
	mIbView.Format = DXGI_FORMAT_R16_UINT;
	mIbView.SizeInBytes = sizeIB;
}