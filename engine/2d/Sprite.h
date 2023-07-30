#pragma once
#include "Vector3.h"
#include "Vector2.h"
#include "Matrix4.h"
#include "Color.h"
#include "DirectXInit.h"
#include <vector>
#include "ConstBuffer.h"
#include <Windows.h>
#include <DirectXMath.h>
#include "Pipeline.h"
#include "Texture.h"

//テクスチャの最大枚数(今は手作業でテクスチャの方と合わせてる)
const int32_t SPRITE_SRV_COUNT = 2056;

struct VertexPosUV
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 uv;
};

struct Sprite
{
	//とりあえずテクスチャーを入れてスプライトを作る
	//以前設定してたフリップとかは別で設定する
	Sprite();
	Sprite(const Texture& tex,const Vector2& anchorpoint = {0.5f,0.5f});

	void SetTexture(const Texture& tex);
	void SetAnchor(const Vector2& anchorpoint);

	void SetPos(const Vector2& pos);
	void SetRotation(const float& rotation);
	void SetColor(const Color& color);

	void SetSize(const Vector2& size);

	void Update();
	void Draw();

	//頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> mVertBuff;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW mVbView{};
	//定数バッファ
	ConstBuffer<ConstBufferDataSprite> mConstBuffer;

	//Z軸回りの回転角
	float mRotation = 0.0f;
	//座標
	Vector3 mPosition = { 0,0,0 };
	//ワールド行列
	DirectX::XMMATRIX mMatWorld;
	//色
	Color mColor = { 1,1,1,1 };

	const Texture *mTEXTURE;

	Vector2 mAnchorpoint = { 0.5f,0.5f };

	bool mIsFlipX = false;
	bool mIsFlipY = false;

	Vector2 mTexLeftTop = {0,0};
	Vector2 mCutSize;

	bool mIsInvisible = false;

	Vector2 mSize = { 0.f,0.f };
protected:
	void Init();

private:
};

struct SpriteCommon
{
	PipelineSet mPipelineSet;
	//射影行列
	DirectX::XMMATRIX mMatProjection{};

	static void Initialize();

	static SpriteCommon sSpriteCommon;
};

//スプライト共通グラフィックコマンドのセット
void SpriteCommonBeginDraw();

void SpriteTransferVertexBuffer(const Sprite& sprite);

SpriteCommon SpriteCommonCreate();
