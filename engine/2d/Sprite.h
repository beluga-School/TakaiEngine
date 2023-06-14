#pragma once
#include "Vector3.h"
#include "Vector2.h"
#include "Matrix4.h"
#include "Color.h"

#include "DirectXInit.h"
#include <vector>
#include "ConstBuffer.h"

#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

#include "Pipeline.h"
#include "Texture.h"

//テクスチャの最大枚数(今は手作業でテクスチャの方と合わせてる)
const int32_t spriteSRVCount = 2056;

struct VertexPosUV
{
	XMFLOAT3 pos;
	XMFLOAT2 uv;
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
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//定数バッファ
	ConstBuffer<ConstBufferDataSprite> constBuffer;

	//Z軸回りの回転角
	float rotation = 0.0f;
	//座標
	Vector3 position = { 0,0,0 };
	//ワールド行列
	XMMATRIX matWorld;
	//色
	Color color = { 1,1,1,1 };

	const Texture *tex;

	Vector2 size;

	Vector2 anchorpoint = { 0.5f,0.5f };

	bool isFlipX = false;
	bool isFlipY = false;

	Vector2 texLeftTop = {0,0};
	Vector2 cutSize;

	bool isInvisible = false;

private:
	void Init();
};

struct SpriteCommon
{
	PipelineSet pipelineSet;
	//射影行列
	XMMATRIX matProjection{};

	static void Initialize();

	static SpriteCommon spriteCommon;
};

//スプライト共通グラフィックコマンドのセット
void SpriteCommonBeginDraw(const SpriteCommon& spriteCommon);

void SpriteTransferVertexBuffer(const Sprite& sprite);

SpriteCommon SpriteCommonCreate();
