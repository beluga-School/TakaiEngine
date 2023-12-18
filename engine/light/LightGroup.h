#pragma once
#include "DirectionalLight.h"
#include "Vector3.h"
#include "ConstBuffer.h"
#include "PointLight.h"
#include "SpotLight.h"
#include <stdlib.h>
#include <memory>
#include "ImguiManager.h"

//ライトの数
static const int32_t sDIRLIGHT_NUM = 3;

//点光源の数
static const int32_t sPOINTLIGHT_NUM = 3;

//丸影の数(今後最大数にしたい)
static const int32_t sCIRCLESHADOW_NUM = 3;

struct LightGroupData
{
	Vector3 pad3;
	float pad;

	DirectionalLight::ConstBufferData mDirLights[sDIRLIGHT_NUM];

	PointLight::ConstBufferData mPointLights[sPOINTLIGHT_NUM];

	CircleShadow::ConstBufferData mCircleShadows[sCIRCLESHADOW_NUM];
};

/*! LightGroup
	ライトデータを管理するクラス
*/
class LightGroup
{
public://変数
	LightGroup() {
		Initialize();
	};

	static LightGroup* Get()
	{
		static LightGroup instance;
		return &instance;
	}
   
	//定数バッファ
	ConstBuffer<LightGroupData> mConstBuff;

public://関数

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

	void LightDebugGUI();

	void PointLightDebug();

	void SpotLightDebug();

	void CircleShadowDebug();

	DirectionalLight mDirLights[sDIRLIGHT_NUM];
	PointLight mPointLights[sDIRLIGHT_NUM];
	CircleShadow mCircleShadows[sCIRCLESHADOW_NUM];

private:

	GUI lightGui = {"lightDebug"};

	bool mDirty = false;
};

