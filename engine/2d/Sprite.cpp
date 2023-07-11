#include "Sprite.h"
#include "Result.h"
#include "Util.h"

using namespace DirectX;

SpriteCommon SpriteCommon::sSpriteCommon;

void SpriteCommon::Initialize()
{
	sSpriteCommon = SpriteCommonCreate();
}

void SpriteTransferVertexBuffer(const Sprite& sprite)
{
	sResult = S_FALSE;

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

	if (sprite.mTEXTURE->mTexBuff)
	{
		D3D12_RESOURCE_DESC resDesc = sprite.mTEXTURE->mGetResDesc;

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
	sResult = sprite.mVertBuff->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, vertices, sizeof(vertices));
	sprite.mVertBuff->Unmap(0, nullptr);
}

void SpriteCommonBeginDraw()
{
	DirectX12* dx12 = DirectX12::Get();
	TextureManager* texM = TextureManager::Get();

	//パイプラインステートの設定
	dx12->mCmdList->SetPipelineState(SpriteCommon::sSpriteCommon.mPipelineSet.mPipelinestate.Get());
	//ルートシグネチャの設定
	dx12->mCmdList->SetGraphicsRootSignature(SpriteCommon::sSpriteCommon.mPipelineSet.mRootsignature.Get());
	//プリミティブ形状を設定
	dx12->mCmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	dx12->mCmdList->SetDescriptorHeaps(1, texM->mSrvHeap.GetAddressOf());
}

SpriteCommon SpriteCommonCreate()
{
	sResult = S_FALSE;

	//スプライトの共通データを作成
	SpriteCommon spriteCommon{};

	//スプライト用のパイプラインを生成
	spriteCommon.mPipelineSet = PipelineManager::GetPipeLine("Sprite");

	//並行投影の射影行列生成
	spriteCommon.mMatProjection = XMMatrixOrthographicOffCenterLH(
		0.0f, (float)Util::WIN_WIDTH, (float)Util::WIN_HEIGHT, 0.0f, 0.0f, 1.0f);

	return spriteCommon;
}

Sprite::Sprite()
{
	this->mTEXTURE = TextureManager::GetTexture("default");

	mPosition.x = 0;
	mPosition.y = 0;
	mPosition.z = 0;
	mRotation = 0;
	mColor = {1,1,1,1};

	Init();
}

Sprite::Sprite(const Texture& tex, const Vector2& anchorpoint)
{
	this->mTEXTURE = &tex;

	mSize = { (float)tex.mGetResDesc.Width,(float)tex.mGetResDesc.Height };

	mCutSize = mSize;

	this->mAnchorpoint = { anchorpoint.x,anchorpoint.y};

	Init();
}

void Sprite::SetTexture(const Texture& tex)
{
	this->mTEXTURE = &tex;

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
	sResult = mConstBuffer.mBuffer->Map(0, nullptr, (void**)&mConstBuffer.mConstBufferData);
	mConstBuffer.mConstBufferData->mat = mMatWorld * SpriteCommon::sSpriteCommon.mMatProjection;

	mConstBuffer.mConstBufferData->color.x = mColor.f4.x;
	mConstBuffer.mConstBufferData->color.y = mColor.f4.y;
	mConstBuffer.mConstBufferData->color.z = mColor.f4.z;
	mConstBuffer.mConstBufferData->color.w = mColor.f4.w;

	mConstBuffer.mBuffer->Unmap(0, nullptr);
}

void Sprite::Draw()
{
	DirectX12* dx12 = DirectX12::Get();
	TextureManager* texM = TextureManager::Get();

	if (mIsInvisible)
	{
		return;
	}

	dx12->mCmdList->IASetVertexBuffers(0, 1, &mVbView);

	dx12->mCmdList->SetGraphicsRootDescriptorTable(1, mTEXTURE->mGpuHandle);

	dx12->mCmdList->SetGraphicsRootConstantBufferView(0, mConstBuffer.mBuffer->GetGPUVirtualAddress());

	dx12->mCmdList->DrawInstanced(4, 1, 0, 0);
}

void Sprite::Init()
{
	DirectX12* dx12 = DirectX12::Get();

	sResult = S_FALSE;

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
	sResult = dx12->mDevice->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(mVertBuff.GetAddressOf()));
	assert(SUCCEEDED(sResult));

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
	mConstBuffer.mConstBufferData->mat =
		XMMatrixOrthographicOffCenterLH(
			0.0f, Util::WIN_WIDTH, Util::WIN_HEIGHT, 0.0f, 0.0f, 1.0f);

	//色指定
	mConstBuffer.mConstBufferData->color = XMFLOAT4(1, 1, 1, 1.0f);

	SpriteTransferVertexBuffer(*this);
}
