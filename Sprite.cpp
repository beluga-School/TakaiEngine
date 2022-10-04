#include "Sprite.h"
#include "Result.h"

Sprite SpriteCreate(DirectX12 dx12, Texture* tex,XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY)
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
	sprite.constBuffer.constBufferData->mat = 
		XMMatrixOrthographicOffCenterLH(
		0.0f, window_width, window_height, 0.0f, 0.0f, 1.0f);

	//色指定
	sprite.constBuffer.constBufferData->color = XMFLOAT4(1, 1, 1, 1.0f);

	sprite.tex = tex;

	sprite.size = { (float)tex->texData.getResDesc.Width,(float)tex->texData.getResDesc.Height };

	sprite.texSize = sprite.size;

	sprite.anchorpoint = anchorpoint;

	sprite.isFlipX = isFlipX;
	sprite.isFlipY = isFlipY;

	SpriteTransferVertexBuffer(sprite);

	return sprite;
}

void SpriteInit(Sprite& sprite, SpriteCommon& spriteCommon, XMFLOAT2 pos, float rotation, XMFLOAT4 color)
{
	sprite.position.x = pos.x;
	sprite.position.y = pos.y;
	sprite.position.z = 0;
	sprite.rotation = rotation;
	sprite.color = color;

	SpriteUpdate(sprite, spriteCommon);
}

void SpriteUpdate(Sprite& sprite, const SpriteCommon& spriteCommon)
{
	//ワールド行列の更新
	sprite.matWorld = XMMatrixIdentity();
	//Z軸回転
	sprite.matWorld *= XMMatrixRotationZ(XMConvertToRadians(sprite.rotation));
	//平行移動
	sprite.matWorld *= XMMatrixTranslation(sprite.position.x, sprite.position.y, sprite.position.z);

	//定数バッファの転送
	result = sprite.constBuffer.buffer->Map(0, nullptr, (void**)&sprite.constBuffer.constBufferData);
	sprite.constBuffer.constBufferData->mat = sprite.matWorld * spriteCommon.matProjection;
	sprite.constBuffer.constBufferData->color = sprite.color;
	sprite.constBuffer.buffer->Unmap(0, nullptr);
}

void SpriteTransferVertexBuffer(const Sprite& sprite)
{
	result = S_FALSE;

	VertexPosUV vertices[] = {
		{{},{0.0f,1.0f}},
		{{},{0.0f,0.0f}},
		{{},{1.0f,1.0f}},
		{{},{1.0f,0.0f}},
	};

	//左下、左上、右下、右上
	enum {
		LB,LT,RB,RT
	};

	float left =	(0.0f - sprite.anchorpoint.x) * sprite.size.x;
	float right =	(1.0f - sprite.anchorpoint.x) * sprite.size.x;
	float top =		(0.0f - sprite.anchorpoint.y) * sprite.size.y;
	float bottom =	(1.0f - sprite.anchorpoint.y) * sprite.size.y;

	if (sprite.isFlipX)
	{
		left = -left;
		right = -right;
	}

	if (sprite.isFlipY)
	{
		top = -top;
		bottom = -bottom;
	}

	vertices[LB].pos = { left,	bottom,	0.0f };
	vertices[LT].pos = { left,	top,	0.0f };
	vertices[RB].pos = { right,	bottom,	0.0f };
	vertices[RT].pos = { right,	top,	0.0f };

	if (sprite.tex->texBuff)
	{
		D3D12_RESOURCE_DESC resDesc = sprite.tex->texData.getResDesc;

		float tex_left = sprite.texLeftTop.x / resDesc.Width;
		float tex_right = (sprite.texLeftTop.x + sprite.texSize.x) / resDesc.Width;
		float tex_top = sprite.texLeftTop.y / resDesc.Height;
		float tex_bottom = (sprite.texLeftTop.y + sprite.texSize.y) / resDesc.Height;

		vertices[LB].uv = { tex_left,tex_bottom };
		vertices[LT].uv = { tex_left,tex_top };
		vertices[RB].uv = { tex_right,tex_bottom };
		vertices[RT].uv = { tex_right,tex_top };
	}

	//頂点バッファへのデータ転送
	VertexPosUV* vertMap = nullptr;
	result = sprite.vertBuff->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, vertices, sizeof(vertices));
	sprite.vertBuff->Unmap(0, nullptr);
}

void SpriteCommonBeginDraw(DirectX12 dx12, const SpriteCommon& spriteCommon)
{
	//パイプラインステートの設定
	dx12.commandList->SetPipelineState(spriteCommon.pipelineSet.pipelinestate.Get());
	//ルートシグネチャの設定
	dx12.commandList->SetGraphicsRootSignature(spriteCommon.pipelineSet.rootsignature.Get());
	//プリミティブ形状を設定
	dx12.commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

void SpriteDraw(const Sprite& sprite, DirectX12 dx12)
{
	if (sprite.isInvisible)
	{
		return;
	}

	dx12.commandList->IASetVertexBuffers(0, 1, &sprite.vbView);

	dx12.commandList->SetDescriptorHeaps(1, &sprite.tex->texData.srvHeap);
	dx12.commandList->SetGraphicsRootDescriptorTable(1, sprite.tex->texData.handle);

	dx12.commandList->SetGraphicsRootConstantBufferView(0, sprite.constBuffer.buffer->GetGPUVirtualAddress());

	dx12.commandList->DrawInstanced(4, 1, 0, 0);
}

void SpriteSetSize(Sprite& sprite, XMFLOAT2 size)
{
	sprite.size = size;
	SpriteTransferVertexBuffer(sprite);
}

SpriteCommon SpriteCommonCreate(DirectX12 dx12)
{
	result = S_FALSE;

	//スプライトの共通データを作成
	SpriteCommon spriteCommon{};

	//スプライト用のパイプラインを生成
	spriteCommon.pipelineSet = CreateSpritePipeline(dx12);

	//並行投影の射影行列生成
	spriteCommon.matProjection = XMMatrixOrthographicOffCenterLH(
		0.0f, (float)window_width, (float)window_height, 0.0f, 0.0f, 1.0f);

	return spriteCommon;
}
