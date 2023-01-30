#pragma once
#include "DirectionalLight.h"
#include "Vector3.h"
#include "ConstBuffer.h"
#include "PointLight.h"

//ライトの数
static const int DirLightNum = 3;

//点光源の数
static const int PointLightNum = 3;

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
	static LightGroup* lightGroup;
	static void SetLight(LightGroup* light) {
		lightGroup = light;
	};
	   
	//定数バッファ
	ConstBuffer<LightGroupData> constBuff;

public://関数
	//インスタンス生成
	static LightGroup* Create();

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
	void SetDirLightActive(int index, const bool& active);

	//平行光源のライト方向をセット
	void SetDirLightDir(int index, const Vector3& lightdir);
	
	//平行光源のライト方向をセット
	void SetDirLightColor(int index, const Vector3& lightcolor);

	//点光源
	
	void SetPointLightActive(int index, const bool& active);
	void SetPointLightPos(int index, const Vector3& pos);
	void SetPointLightColor(int index, const Vector3& color);
	void SetPointLightAtten(int index, const Vector3& atten);

	//標準のライト設定
	void DefaultLightSet();

private:
	//環境光の色
	Vector3 ambienColor = { 1,1,1 };

	DirectionalLight dirLights[DirLightNum];
	PointLight pointLights[DirLightNum];

	bool dirty = false;
};

