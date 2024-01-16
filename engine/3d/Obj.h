#pragma once
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include "DirectXInit.h"
#include "Model.h"
#include "Texture.h"
#include <memory>
#include "Matrix4.h"
#include "LightGroup.h"
#include "Camera.h"
#include "Pipeline.h"
#include "TEasing.h"
#include "Quaternion.h"

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
	Matrix4 viewproj;	//ビュープロ行列
	Matrix4 world;		//ワールド行列
	Vector3 cameraPos;	//カメラ座標(ワールド行列)
	float pad1;
	Vector2 tiling;		//タイリング数
	Vector2 offset;		//オフセット数
};

struct ConstBufferDataOutLine {
	Float4 color;
	float thickness;
};

struct ConstBufferDisolve {
	float value;
};

//特殊描画時の定数
namespace SpecialDraw
{
	typedef int32_t DISOLVE;
	typedef int32_t TEXTUREBLEND;

	static DISOLVE DISOLVE_;
	static TEXTUREBLEND TEXTUREBLEND_;
}

/*! Obj3d
	3Dオブジェクト
*/
class Obj3d
{
public:

	virtual ~Obj3d() {};

	enum class RotMode
	{
		Eular,
		Quaternion,
	};

	RotMode rotmode = RotMode::Eular;

	ConstBuffer<ConstBufferDataTransform> constBufferT;
	ConstBuffer<ConstBufferBrightness> constBufferB;

	ConstBuffer<ConstBufferDataB1> constBufferB1;
	ConstBuffer<ConstBufferDataOutLine> constBufferOutLine;

	ConstBuffer<ConstBufferDisolve> constBufferDisolve;

	Vector3 scale = { 1.0f,1.0f,1.0f };
	Vector3 rotation = { 0,0,0 };
	Vector3 position = { 0,0,0 };

	Quaternion quaternion = Quaternion::IdentityQuaternion();

	Float4 color_ = { 1.0f,1.0f,1.0f,1.0f };

	Matrix4 matWorld;

	Obj3d* parent = nullptr;

	const Texture* TEXTURE = nullptr;

	const Model* MODEL = nullptr;

	//描画フラグ
	bool mIsVisiable = true;

	//ディゾルブの進行度(0～1で管理)
	float disolveVal = 0;

	float transports = 0.0f;

	Vector2 mTiling = { 1,1 };
	Vector2 mTexOffset = { 0,0 };

	Vector3 mModelOffset = { 0,0,0 };

public:
	//初期化
	void Initialize();
	//モデル設定
	void SetModel(const Model* model);
	//テクスチャ設定
	void SetTexture(const Texture* texture);
	//更新
	void Update(const Camera& camera);
	//ワールド座標を更新
	Vector3 GetWorldTrans();
	//テクスチャを使用して描画
	void Draw();
	//マテリアルを使用して描画
	void DrawMaterial();

	///---特殊描画
	//アウトラインで使用する描画
	void DrawOutLine();

	//テクスチャブレンドで使用する描画
	//namespaceを使っている理由は、同じ名前で引数の数を変えて運用するために、最初に書いた引数のクラスの形で判別できるようにしている
	void DrawSpecial(SpecialDraw::TEXTUREBLEND drawkey, const Texture& subTex, const Texture& maskTex);
	void DrawSpecial(SpecialDraw::DISOLVE drawkey, const Texture& maskTex);

	///---ここら辺は関数をひとつにして、定数で振る舞いを変えるような設計にしたい
	///定数の中身でオーバーロード先を変えれる感じにしたい

	//アウトラインの設定回り
	void SetOutLineState(const Float4& color, float thickness);
	void SetOutLineAlpha(const float& alpha);

protected:
	//アウトライン周りの設定
	Float4 mOutLineColor = { 0,0,0,1.0f };
	float mOutLineThickness = 0.0f;
};