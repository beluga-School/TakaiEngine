#include "Sprite.h"
#include "Result.h"
#include "Util.h"

SpriteCommon SpriteCommon::spriteCommon;

void SpriteCommon::Initialize()
{
	spriteCommon = SpriteCommonCreate();
}

//void SpriteUpdate(Sprite& sprite, const SpriteCommon& spriteCommon)
//{
//	//ワールド行列の更新
//	sprite.matWorld = XMMatrixIdentity();
//	//Z軸回転
//	sprite.matWorld *= XMMatrixRotationZ(XMConvertToRadians(sprite.rotation));
//	//平行移動
//	sprite.matWorld *= XMMatrixTranslation(sprite.position.x, sprite.position.y, sprite.position.z);
//
//	//定数バッファの転送
//	result = sprite.constBuffer.buffer->Map(0, nullptr, (void**)&sprite.constBuffer.constBufferData);
//	sprite.constBuffer.constBufferData->mat = sprite.matWorld * spriteCommon.matProjection;
//	
//	sprite.constBuffer.constBufferData->color.x = sprite.color.f4.vec.x;
//	sprite.constBuffer.constBufferData->color.y = sprite.color.f4.vec.y;
//	sprite.constBuffer.constBufferData->color.z = sprite.color.f4.vec.z;
//	sprite.constBuffer.constBufferData->color.w = sprite.color.f4.w;
//	
//	sprite.constBuffer.buffer->Unmap(0, nullptr);
//}

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

	vertices[LB].pos = { 0.0f,	sprite.size.y,	0.0f };
	vertices[LT].pos = { 0.0f,	0.0f,	0.0f };
	vertices[RB].pos = { sprite.size.x,	sprite.size.y,	0.0f };
	vertices[RT].pos = { sprite.size.x,	0.0f,	0.0f };

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
		D3D12_RESOURCE_DESC resDesc = sprite.tex->getResDesc;

		float tex_left = sprite.texLeftTop.x / resDesc.Width;
		float tex_right = (sprite.texLeftTop.x + sprite.cutSize.x) / resDesc.Width;
		float tex_top = sprite.texLeftTop.y / resDesc.Height;
		float tex_bottom = (sprite.texLeftTop.y + sprite.cutSize.y) / resDesc.Height;

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

void SpriteCommonBeginDraw(const SpriteCommon& spriteCommon)
{
	DirectX12* dx12 = DirectX12::GetInstance();
	TextureManager* texM = TextureManager::Get();

	//パイプラインステートの設定
	dx12->commandList->SetPipelineState(spriteCommon.pipelineSet.pipelinestate.Get());
	//ルートシグネチャの設定
	dx12->commandList->SetGraphicsRootSignature(spriteCommon.pipelineSet.rootsignature.Get());
	//プリミティブ形状を設定
	dx12->commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	dx12->commandList->SetDescriptorHeaps(1, texM->srvHeap.GetAddressOf());
}

//void SpriteDraw(const Sprite& sprite)
//{
//	DirectX12* dx12 = DirectX12::GetInstance();
//	TextureManager* texM = TextureManager::GetInstance();
//
//	if (sprite.isInvisible)
//	{
//		return;
//	}
//
//	dx12->commandList->IASetVertexBuffers(0, 1, &sprite.vbView);
//
//	dx12->commandList->SetGraphicsRootDescriptorTable(1, sprite.tex->gpuHandle);
//
//	dx12->commandList->SetGraphicsRootConstantBufferView(0, sprite.constBuffer.buffer->GetGPUVirtualAddress());
//
//	dx12->commandList->DrawInstanced(4, 1, 0, 0);
//}

void SpriteSetSize(Sprite& sprite, XMFLOAT2 size)
{
	sprite.size = size;
	SpriteTransferVertexBuffer(sprite);
}

SpriteCommon SpriteCommonCreate()
{
	result = S_FALSE;

	//スプライトの共通データを作成
	SpriteCommon spriteCommon{};

	//スプライト用のパイプラインを生成
	spriteCommon.pipelineSet = CreateSpritePipeline();

	//並行投影の射影行列生成
	spriteCommon.matProjection = XMMatrixOrthographicOffCenterLH(
		0.0f, (float)Util::window_width, (float)Util::window_height, 0.0f, 0.0f, 1.0f);

	return spriteCommon;
}

Sprite::Sprite()
{
	this->tex = TextureManager::GetTexture("default");

	position.x = 0;
	position.y = 0;
	position.z = 0;
	rotation = 0;
	color = {1,1,1,1};

	Init();
}

Sprite::Sprite(Texture* tex, Vector2 anchorpoint)
{
	this->tex = tex;

	size = { (float)tex->getResDesc.Width,(float)tex->getResDesc.Height };

	cutSize = size;

	this->anchorpoint = { anchorpoint .x,anchorpoint .y};

	Init();
}

void Sprite::SetTexture(Texture* tex)
{
	this->tex = tex;

	size = { (float)tex->getResDesc.Width,(float)tex->getResDesc.Height };

	cutSize = size;

	SpriteTransferVertexBuffer(*this);
}

void Sprite::SetAnchor(Vector2 anchorpoint)
{
	this->anchorpoint = { anchorpoint.x,anchorpoint.y };

	SpriteTransferVertexBuffer(*this);
}

void Sprite::SetPos(Vector2 pos)
{
	position.x = pos.x;
	position.y = pos.y;
	position.z = 0;
}

void Sprite::SetRotation(float rotation)
{
	this->rotation = rotation;
}

void Sprite::SetColor(Color color)
{
	this->color = color;
}

void Sprite::Update()
{
	//ワールド行列の更新
	matWorld = XMMatrixIdentity();
	//Z軸回転
	matWorld *= XMMatrixRotationZ(XMConvertToRadians(rotation));
	//平行移動
	matWorld *= XMMatrixTranslation(position.x, position.y, position.z);

	//定数バッファの転送
	result = constBuffer.buffer->Map(0, nullptr, (void**)&constBuffer.constBufferData);
	constBuffer.constBufferData->mat = matWorld * SpriteCommon::spriteCommon.matProjection;

	constBuffer.constBufferData->color.x = color.f4.vec.x;
	constBuffer.constBufferData->color.y = color.f4.vec.y;
	constBuffer.constBufferData->color.z = color.f4.vec.z;
	constBuffer.constBufferData->color.w = color.f4.w;

	constBuffer.buffer->Unmap(0, nullptr);
}

void Sprite::Draw()
{
	DirectX12* dx12 = DirectX12::GetInstance();
	TextureManager* texM = TextureManager::Get();

	if (isInvisible)
	{
		return;
	}

	dx12->commandList->IASetVertexBuffers(0, 1, &vbView);

	dx12->commandList->SetGraphicsRootDescriptorTable(1, tex->gpuHandle);

	dx12->commandList->SetGraphicsRootConstantBufferView(0, constBuffer.buffer->GetGPUVirtualAddress());

	dx12->commandList->DrawInstanced(4, 1, 0, 0);
}

void Sprite::Init()
{
	DirectX12* dx12 = DirectX12::GetInstance();

	result = S_FALSE;

	//新規スプライトを生成
	//Sprite sprite{};

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
	result = dx12->device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(vertBuff.GetAddressOf()));

	//頂点バッファへのデータ転送
	/*VertexPosUV* vertMap = nullptr;
	result = sprite.vertBuff->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, vertices, sizeof(vertices));
	sprite.vertBuff->Unmap(0, nullptr);*/

	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(vertices);
	vbView.StrideInBytes = sizeof(vertices[0]);

	//定数バッファの設定
	//平行投影行列
	constBuffer.constBufferData->mat =
		XMMatrixOrthographicOffCenterLH(
			0.0f, Util::window_width, Util::window_height, 0.0f, 0.0f, 1.0f);

	//色指定
	constBuffer.constBufferData->color = XMFLOAT4(1, 1, 1, 1.0f);

	SpriteTransferVertexBuffer(*this);
}
