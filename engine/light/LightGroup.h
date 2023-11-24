#pragma once
#include "DirectionalLight.h"
#include "Vector3.h"
#include "ConstBuffer.h"
#include "PointLight.h"
#include <stdlib.h>
#include <memory>

//ライトの数
static const int32_t sDIRLIGHT_NUM = 3;

//点光源の数
static const int32_t sPOINTLIGHT_NUM = 3;

struct LightGroupData
{
	Vector3 pad3;
	float pad;

	DirectionalLight::ConstBufferData mDirLights[sDIRLIGHT_NUM];

	PointLight::ConstBufferData mPointLights[sPOINTLIGHT_NUM];
};

/*! LightGroup
	ライトデータを管理するクラス
*/
class LightGroup
{
public://変数
	

	//全てのオブジェクトで共通のライトデータ
	static std::unique_ptr<LightGroup> sLightGroup;
	   
	//定数バッファ
	ConstBuffer<LightGroupData> mConstBuff;

public://関数
	//インスタンス生成
	static void Create();

	//初期化
	void Initialize();
	//更新
	void Update();
	//描画コマンド
	void Draw(const UINT& index);

	//定数バッファを転送
	void TransferBuffer();

	//平行光源
	//平行光源の有効フラグをセット
	void SetDirLightActive(const int32_t& index, const bool& active);

	//平行光源のライト方向をセット
	void SetDirLightDir(const int32_t& index, const Vector3& lightdir);
	
	//平行光源のライト方向をセット
	void SetDirLightColor(const int32_t& index, const Vector3& lightcolor);

	//点光源
	void SetPointLightActive(const int32_t& index, const bool& active);
	void SetPointLightPos(const int32_t& index, const Vector3& pos);
	void SetPointLightColor(const int32_t& index, const Vector3& color);
	void SetPointLightAtten(const int32_t& index, const Vector3& atten);

	//標準のライト設定
	void DefaultLightSet();

	DirectionalLight mDirLights[sDIRLIGHT_NUM];
	PointLight mPointLights[sDIRLIGHT_NUM];

private:

	bool mDirty = false;
};

