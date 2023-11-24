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

/*! Sprite
	テクスチャを2D描画するクラス
*/
class Sprite
{
public:
	//引数無しコンストラクタ デフォルトテクスチャを代入しデータを初期化
	Sprite();

	//引数有りコンストラクタ 指定されたテクスチャで生成
	Sprite(const Texture& tex,const Vector2& anchorpoint = {0.5f,0.5f});

	//テクスチャを設定
	void SetTexture(const Texture& tex);

	//元の大きさを基準にサイズを割合で変えて頂点バッファを送りなおす
	void SetSizeRate(const Vector2& sizeRate);

	//頂点バッファを送りなおす必要があるものはセッターとゲッターを使用する
	///セッター

	//アンカーポイントを決定
	void SetAnchor(const Vector2& anchorpoint);

	//サイズを書き換える
	void SetSize(const Vector2& size);

	///ゲッター
	Vector2 GetSize();

	//テクスチャの元の大きさを取得
	//拡縮処理などを書くときに使用する
	Vector2 GetInitSize();

	//初期化
	void Init();

	//更新
	void Update();
	
	//描画
	void Draw();

	//頂点バッファへデータを送信
	void SpriteTransferVertexBuffer();

public:
	//座標
	Vector2 mPosition = { 0,0 };
	//Z軸回りの回転角
	float mRotation = 0.0f;
	//色
	Color mColor = { 1,1,1,1 };

	//頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> mVertBuff;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW mVbView{};
	//定数バッファ
	ConstBuffer<ConstBufferDataSprite> mConstBuffer;

	//ワールド行列
	DirectX::XMMATRIX mMatWorld;
	
	//反転
	bool mIsFlipX = false;
	bool mIsFlipY = false;

	Vector2 mCutSize{};

protected:
	bool mIsInvisible = false;

	Vector2 mInitSize = {};

	Vector2 mSize = { 0.f,0.f };

	Vector2 mAnchorpoint = { 0.5f,0.5f };

	Vector2 mTexLeftTop = { 0,0 };

	const Texture* mTEXTURE;
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

SpriteCommon SpriteCommonCreate();
