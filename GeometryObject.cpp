#include "GeometryObject.h"
#include "DirectXInit.h"

void GeometryObject::CreateModel()
{
	DirectX12 *dx12 = DirectX12::GetInstance();

	verticesPoint[vertexCount] = {
		{0.0f,0.0f,0.0f}
	};

	//頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	//UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * vertices.size());

	//頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapProp{};		//ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPUへの転送用

	//リソース設定
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	//頂点データ全体のサイズ
	resDesc.Width = sizeof(verticesPoint);
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//頂点バッファの生成
	//ComPtrにしたらダメだった マップ処理に使ってるから？
	ID3D12Resource* vertBuff;
	result = dx12->device->CreateCommittedResource(
		&heapProp,	//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc,	//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	//GPU状のバッファに対応した仮想メモリ(メインメモリ上)を取得
	VertexPos* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	//全頂点に対して
	for (int i = 0; i < vertexCount; i++) {
		vertMap[i] = verticesPoint[i];	//	座標をコピー
	}
	//繋がりを解除
	vertBuff->Unmap(0, nullptr);

	//GPU仮想アドレス
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	//頂点バッファのサイズ
	vbView.SizeInBytes = sizeof(verticesPoint);
	//頂点1つ分のデータサイズ
	vbView.StrideInBytes = sizeof(verticesPoint[0]);
}

void GeometryObject::Update(XMMATRIX& matView, XMMATRIX& matProjection)
{
	constBufferGeometry.constBufferData->mat = matView * matProjection;
}

void GeometryObject::Draw()
{
	DirectX12* dx12 = DirectX12::GetInstance();

	//頂点バッファの設定
	dx12->commandList->IASetVertexBuffers(0, 1, &vbView);

	//定数バッファビュー(CBV)の設定コマンド
	//dx12->commandList->SetGraphicsRootConstantBufferView(0, constBufferMaterial.buffer->GetGPUVirtualAddress());
	//commandList->SetGraphicsRootConstantBufferView(0, constBufferM.buffer->GetGPUVirtualAddress());

	dx12->commandList->SetGraphicsRootConstantBufferView(0, constBufferGeometry.buffer->GetGPUVirtualAddress());

	//描画コマンド
	dx12->commandList->DrawInstanced(_countof(verticesPoint), 1, 0, 0);
}
