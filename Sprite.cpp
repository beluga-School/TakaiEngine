#include "Sprite.h"
#include "Result.h"

Sprite SpriteCreate(DirectX12 dx12)
{
	result = S_FALSE;

	//新規スプライトを生成
	Sprite sprite{};

	//頂点データ
	VertexPosUV vertices[] = {
		//x      y      z        u    v
		{{  0.0f,100.0f,  0.0f},{0.0f,1.0f}},//左下
		{{  0.0f,  0.0f,  0.0f},{0.0f,0.0f}},//左上
		{{100.0f,100.0f,  0.0f},{1.0f,1.0f}},//右下
		{{100.0f,  0.0f,  0.0f},{1.0f,0.0f}},//右上
	};

	//頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapProp{};		//ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPUへの転送用

	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	//ここ悪いかもしれん
	resDesc.Width = sizeof(vertices);	//頂点データ全体のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//頂点バッファ生成
	result = dx12.device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&sprite.vertBuff));

	//頂点バッファへのデータ転送
	VertexPosUV* vertMap = nullptr;
	result = sprite.vertBuff->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, vertices, sizeof(vertices));
	sprite.vertBuff->Unmap(0, nullptr);

	sprite.vbView.BufferLocation = sprite.vertBuff->GetGPUVirtualAddress();
	sprite.vbView.SizeInBytes = sizeof(vertices);
	sprite.vbView.StrideInBytes = sizeof(vertices[0]);

	//定数バッファの設定
	//平行投影行列
	sprite.constBufferT.constBufferData->mat = XMMatrixOrthographicOffCenterLH(
		0.0f, window_width, window_height, 0.0f, 0.0f, 1.0f);

	//色指定
	sprite.constBufferM.constBufferData->color = XMFLOAT4(1, 1, 1, 1.0f);

	return sprite;
}

void SpriteCommonBeginDraw(DirectX12 dx12, const PipelineSet& pipelineSet, Texture &tex)
{
	//パイプラインステートの設定
	dx12.commandList->SetPipelineState(pipelineSet.pipelinestate.Get());
	//ルートシグネチャの設定
	dx12.commandList->SetGraphicsRootSignature(pipelineSet.rootsignature.Get());
	//プリミティブ形状を設定
	dx12.commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//dx12.commandList->SetDescriptorHeaps(1, &tex.srvHeap);
}

void SpriteDraw(const Sprite& sprite, DirectX12 dx12, Texture& tex)
{
	dx12.commandList->IASetVertexBuffers(0, 1, &sprite.vbView);

	dx12.commandList->SetDescriptorHeaps(1, &tex.srvHeap);
	dx12.commandList->SetGraphicsRootDescriptorTable(1, tex.GetHandle());

	dx12.commandList->SetGraphicsRootConstantBufferView(0, sprite.constBufferM.buffer->GetGPUVirtualAddress());
	dx12.commandList->SetGraphicsRootConstantBufferView(2, sprite.constBufferT.buffer->GetGPUVirtualAddress());

	dx12.commandList->DrawInstanced(4, 1, 0, 0);
}
