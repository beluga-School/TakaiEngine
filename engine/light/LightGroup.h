#pragma once
#include "DirectionalLight.h"
#include "Vector3.h"
#include "ConstBuffer.h"
#include "PointLight.h"
#include <stdlib.h>
#include <memory>

//ライトの数
static const int32_t sDirLightNum = 3;

//点光源の数
static const int32_t sPointLightNum = 3;

struct LightGroupData
{
	Vector3 mAmbienColor;
	float mPad1;

	DirectionalLight::ConstBufferData mDirLights[sDirLightNum];

	PointLight::ConstBufferData mPointLights[sPointLightNum];
};

class LightGroup
{
public://変数
	

	//全てのオブジェクトで共通のライトデータ
	static std::unique_ptr<LightGroup> mLightGroup;
	/*static void SetLight(std::unique_ptr<LightGroup> light) {
		lightGroup = light;
	};*/
	   
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
	//ライトの色を設定
	void SetAmbientColor(const Vector3& color);

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

	DirectionalLight mDirLights[sDirLightNum];
	PointLight mPointLights[sDirLightNum];

private:
	//環境光の色
	Vector3 mAmbienColor = { 1,1,1 };

	bool mDirty = false;
};

