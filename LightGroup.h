#pragma once
#include "DirectionalLight.h"
#include "Vector3.h"
#include "ConstBuffer.h"

//ライトの数
static const int DirLightNum = 3;

struct LightGroupData
{
	Vector3 ambienColor;
	float pad1;

	DirectionalLight dirLights[DirLightNum];
};

class LightGroup
{
public://変数
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

	//ライトの色を設定
	void SetAmbientColor(Vector3& color);

	//平行光源の有効フラグをセット
	void SetDirLightActive(int index,bool active);

	//平行光源のライト方向をセット
	void SetDirLightDir(int index, Vector3 lightdir);
	
	//平行光源のライト方向をセット
	void SetDirLightColor(int index, Vector3 lightcolor);

	//標準のライト設定
	void DefaultLightSet();

private:
	//環境光の色
	Vector3 ambienColor = { 1,1,1 };

	DirectionalLight dirLights[DirLightNum];

	bool dirty = false;
};

