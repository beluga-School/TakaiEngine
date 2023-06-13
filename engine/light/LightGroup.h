#pragma once
#include "DirectionalLight.h"
#include "Vector3.h"
#include "ConstBuffer.h"
#include "PointLight.h"
#include <stdlib.h>
#include <memory>

//ライトの数
static const int32_t DirLightNum = 3;

//点光源の数
static const int32_t PointLightNum = 3;

struct LightGroupData
{
	Vector3 ambienColor;
	float pad1;

	DirectionalLight::ConstBufferData dirLights[DirLightNum];

	PointLight::ConstBufferData pointLights[PointLightNum];
};

class LightGroup
{
public://変数
	

	//全てのオブジェクトで共通のライトデータ
	static std::unique_ptr<LightGroup> lightGroup;
	/*static void SetLight(std::unique_ptr<LightGroup> light) {
		lightGroup = light;
	};*/
	   
	//定数バッファ
	ConstBuffer<LightGroupData> constBuff;

public://関数
	//インスタンス生成
	static void Create();

	//初期化
	void Initialize();
	//更新
	void Update();
	//描画コマンド
	void Draw(UINT index);

	//定数バッファを転送
	void TransferBuffer();

	//平行光源
	//ライトの色を設定
	void SetAmbientColor(const Vector3& color);

	//平行光源の有効フラグをセット
	void SetDirLightActive(int32_t index, const bool& active);

	//平行光源のライト方向をセット
	void SetDirLightDir(int32_t index, const Vector3& lightdir);
	
	//平行光源のライト方向をセット
	void SetDirLightColor(int32_t index, const Vector3& lightcolor);

	//点光源
	
	void SetPointLightActive(int32_t index, const bool& active);
	void SetPointLightPos(int32_t index, const Vector3& pos);
	void SetPointLightColor(int32_t index, const Vector3& color);
	void SetPointLightAtten(int32_t index, const Vector3& atten);

	//標準のライト設定
	void DefaultLightSet();

	DirectionalLight dirLights[DirLightNum];
	PointLight pointLights[DirLightNum];

private:
	//環境光の色
	Vector3 ambienColor = { 1,1,1 };

	bool dirty = false;
};

