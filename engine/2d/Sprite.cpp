#include "Sprite.h"
#include "Result.h"
#include "Util.h"

SpriteCommon SpriteCommon::mSpriteCommon;

void SpriteCommon::Initialize()
{
	mSpriteCommon = SpriteCommonCreate();
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

	vertices[LB].pos = { 0.0f,	sprite.mSize.y,	0.0f };
	vertices[LT].pos = { 0.0f,	0.0f,	0.0f };
	vertices[RB].pos = { sprite.mSize.x,	sprite.mSize.y,	0.0f };
	vertices[RT].pos = { sprite.mSize.x,	0.0f,	0.0f };

	float left =	(0.0f - sprite.mAnchorpoint.x) * sprite.mSize.x;
	float right =	(1.0f - sprite.mAnchorpoint.x) * sprite.mSize.x;
	float top =		(0.0f - sprite.mAnchorpoint.y) * sprite.mSize.y;
	float bottom =	(1.0f - sprite.mAnchorpoint.y) * sprite.mSize.y;

	if (sprite.mIsFlipX)
	{
		left = -left;
		right = -right;
	}

	if (sprite.mIsFlipY)
	{
		top = -top;
		bottom = -bottom;
	}

	vertices[LB].pos = { left,	bottom,	0.0f };
	vertices[LT].pos = { left,	top,	0.0f };
	vertices[RB].pos = { right,	bottom,	0.0f };
	vertices[RT].pos = { right,	top,	0.0f };

	if (sprite.mTexture->mTexBuff)
	{
		D3D12_RESOURCE_DESC resDesc = sprite.mTexture->mGetResDesc;

		float tex_left = sprite.mTexLeftTop.x / resDesc.Width;
		float tex_right = (sprite.mTexLeftTop.x + sprite.mCutSize.x) / resDesc.Width;
		float tex_top = sprite.mTexLeftTop.y / resDesc.Height;
		float tex_bottom = (sprite.mTexLeftTop.y + sprite.mCutSize.y) / resDesc.Height;

		vertices[LB].uv = { tex_left,tex_bottom };
		vertices[LT].uv = { tex_left,tex_top };
		vertices[RB].uv = { tex_right,tex_bottom };
		vertices[RT].uv = { tex_right,tex_top };
	}

	//頂点バッファへのデータ転送
	VertexPosUV* vertMap = nullptr;
	result = sprite.mVertBuff->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, vertices, sizeof(vertices));
	sprite.mVertBuff->Unmap(0, nullptr);
}

void SpriteCommonBeginDraw(const SpriteCommon& spriteCommon)
{
	DirectX12* dx12 = DirectX12::Get();
	TextureManager* texM = TextureManager::Get();

	//パイプラインステートの設定
	dx12->commandList->SetPipelineState(spriteCommon.mPipelineSet.pipelinestate.Get());
	//ルートシグネチャの設定
	dx12->commandList->SetGraphicsRootSignature(spriteCommon.mPipelineSet.rootsignature.Get());
	//プリミティブ形状を設定
	dx12->commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	dx12->commandList->SetDescriptorHeaps(1, texM->mSrvHeap.GetAddressOf());
}

SpriteCommon SpriteCommonCreate()
{
	result = S_FALSE;

	//スプライトの共通データを作成
	SpriteCommon spriteCommon{};

	//スプライト用のパイプラインを生成
	spriteCommon.mPipelineSet = CreateSpritePipeline();

	//並行投影の射影行列生成
	spriteCommon.mMatProjection = XMMatrixOrthographicOffCenterLH(
		0.0f, (float)Util::window_width, (float)Util::window_height, 0.0f, 0.0f, 1.0f);

	return spriteCommon;
}

Sprite::Sprite()
{
	this->mTexture = TextureManager::GetTexture("default");

	mPosition.x = 0;
	mPosition.y = 0;
	mPosition.z = 0;
	mRotation = 0;
	mColor = {1,1,1,1};

	Init();
}

Sprite::Sprite(const Texture& tex, const Vector2& anchorpoint)
{
	this->mTexture = &tex;

	mSize = { (float)tex.mGetResDesc.Width,(float)tex.mGetResDesc.Height };

	mCutSize = mSize;

	this->mAnchorpoint = { anchorpoint.x,anchorpoint.y};

	Init();
}

void Sprite::SetTexture(const Texture& tex)
{
	this->mTexture = &tex;

	mSize = { (float)tex.mGetResDesc.Width,(float)tex.mGetResDesc.Height };

	mCutSize = mSize;

	SpriteTransferVertexBuffer(*this);
}

void Sprite::SetAnchor(const Vector2& anchorpoint)
{
	this->mAnchorpoint = { anchorpoint.x,anchorpoint.y };

	SpriteTransferVertexBuffer(*this);
}

void Sprite::SetPos(const Vector2& pos)
{
	mPosition.x = pos.x;
	mPosition.y = pos.y;
	mPosition.z = 0;
}

void Sprite::SetRotation(const float& rotation)
{
	mRotation = rotation;
}

void Sprite::SetColor(const Color& color)
{
	mColor = color;
}

void Sprite::SetSize(const Vector2& size)
{
	mSize = size;
	SpriteTransferVertexBuffer(*this);
}

void Sprite::Update()
{
	//ワールド行列の更新
	mMatWorld = XMMatrixIdentity();
	//Z軸回転
	mMatWorld *= XMMatrixRotationZ(XMConvertToRadians(mRotation));
	//平行移動
	mMatWorld *= XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z);

	//定数バッファの転送
	result = mConstBuffer.buffer->Map(0, nullptr, (void**)&mConstBuffer.constBufferData);
	mConstBuffer.constBufferData->mat = mMatWorld * SpriteCommon::mSpriteCommon.mMatProjection;

	mConstBuffer.constBufferData->color.x = mColor.f4.vec.x;
	mConstBuffer.constBufferData->color.y = mColor.f4.vec.y;
	mConstBuffer.constBufferData->color.z = mColor.f4.vec.z;
	mConstBuffer.constBufferData->color.w = mColor.f4.w;

	mConstBuffer.buffer->Unmap(0, nullptr);
}

void Sprite::Draw()
{
	DirectX12* dx12 = DirectX12::Get();
	TextureManager* texM = TextureManager::Get();

	if (mIsInvisible)
	{
		return;
	}

	dx12->commandList->IASetVertexBuffers(0, 1, &mVbView);

	dx12->commandList->SetGraphicsRootDescriptorTable(1, mTexture->mGpuHandle);

	dx12->commandList->SetGraphicsRootConstantBufferView(0, mConstBuffer.buffer->GetGPUVirtualAddress());

	dx12->commandList->DrawInstanced(4, 1, 0, 0);
}

void Sprite::Init()
{
	DirectX12* dx12 = DirectX12::Get();

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
		IID_PPV_ARGS(mVertBuff.GetAddressOf()));

	//頂点バッファへのデータ転送
	/*VertexPosUV* vertMap = nullptr;
	result = sprite.vertBuff->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, vertices, sizeof(vertices));
	sprite.vertBuff->Unmap(0, nullptr);*/

	mVbView.BufferLocation = mVertBuff->GetGPUVirtualAddress();
	mVbView.SizeInBytes = sizeof(vertices);
	mVbView.StrideInBytes = sizeof(vertices[0]);

	//定数バッファの設定
	//平行投影行列
	mConstBuffer.constBufferData->mat =
		XMMatrixOrthographicOffCenterLH(
			0.0f, Util::window_width, Util::window_height, 0.0f, 0.0f, 1.0f);

	//色指定
	mConstBuffer.constBufferData->color = XMFLOAT4(1, 1, 1, 1.0f);

	SpriteTransferVertexBuffer(*this);
}
