#pragma once
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include "DirectXInit.h"
#include "Model.h"
#include "Texture.h"
#include <memory>
#include "Matrix4.h"
#include "LightGroup.h"
#include "ViewProjection.h"
#include "Pipeline.h"
#include "TEasing.h"

struct ConstBufferDataB1
{
	Vector3 ambient;	//アンビエント係数
	float pad1;			//パディング
	Vector3 diffuse;	//ディフューズ係数
	float pad2;			//パディング
	Vector3 specular;	//スペキュラー係数
	float alpha;
};

struct ConstBufferBrightness
{
	Float4 brightness;		//カラー
};

struct ConstBufferDataTransform {
	//Matrix4 mat;	//3D変換行列
	Matrix4 viewproj;	//ビュープロ行列
	Matrix4 world;		//ワールド行列
	Vector3 cameraPos;	//カメラ座標(ワールド行列)
	float pad1;
	Vector3 cameraDir;  //カメラの視線ベクトル <-これ怪しすぎないか？？？？
	float pad2;
};

struct ConstBufferDataOutLine {
	Float4 color;
	float thickness;
};

struct ConstBufferDisolve {
	float value;
};

struct ConstBufferCamPlayer {
	Vector3 playerPos;
	float radius;
};

//特殊描画時の定数
namespace SpecialDraw
{
	typedef int32_t DISOLVE;
	typedef int32_t TEXTUREBLEND;

	static DISOLVE DISOLVE_;
	static TEXTUREBLEND TEXTUREBLEND_;
}

class Obj3d
{
public:

	ConstBuffer<ConstBufferDataTransform> constBufferT;
	ConstBuffer<ConstBufferBrightness> constBufferB;

	ConstBuffer<ConstBufferDataB1> constBufferB1;
	ConstBuffer<ConstBufferDataOutLine> constBufferOutLine;

	ConstBuffer<ConstBufferDisolve> constBufferDisolve;
	ConstBuffer<ConstBufferCamPlayer> constBufferCamPlayer;

	Vector3 scale = { 1.0f,1.0f,1.0f };
	Vector3 rotation = { 0,0,0 };
	Vector3 position = { 0,0,0 };

	Float4 color_ = { 1.0f,1.0f,1.0f,1.0f };

	Matrix4 matWorld;

	Obj3d* parent = nullptr;
	
	const Texture *TEXTURE = nullptr;

	const Model* MODEL = nullptr;

	//描画フラグ
	bool mIsVisiable = true;

	//ディゾルブの進行度(0～1で管理)
	float disolveVal = 0;

public:
	void Initialize();
	void SetModel(const Model *model);
	void SetTexture(const Texture *texture);
	void Update(const Camera& camera);

	Vector3 GetWorldTrans();

	void Draw();
	void DrawMaterial();

	///---特殊描画
	//アウトラインで使用する描画
	void DrawOutLine();

	//テクスチャブレンドで使用する描画
	void DrawSpecial(SpecialDraw::TEXTUREBLEND drawkey,const Texture& subTex,const Texture& maskTex);
	void DrawSpecial(SpecialDraw::DISOLVE drawkey,const Texture& maskTex);
	
	///---ここら辺は関数をひとつにして、定数で振る舞いを変えるような設計にしたい
	///定数の中身でオーバーロード先を変えれる感じにしたい

	void SetOutLineState(const Float4& color,float thickness);
	void SetOutLineAlpha(const float& alpha);

	TEasing::easeTimer transparentTimer = 0.5f;
protected:
	//アウトライン周りの設定
	Float4 mOutLineColor = {0,0,0,1.0f};
	float mOutLineThickness = 0.0f;
};