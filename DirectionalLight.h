#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "ConstBuffer.h"

struct LightConstBufferData
{
	Vector3 direction;
	float pad;
	Vector3 color;
};

class DirectionalLight
{
public://変数
	Vector3 direction = { 1,0,0 };
	Vector3 color = { 1,1,1 };

public://関数
	static DirectionalLight* Create() {
		//3Dオブジェクトのインスタンスを生成
		DirectionalLight* instance = new DirectionalLight();
		//初期化
		instance->Initialize();
		//生成したインスタンスを返す
		return instance;
	};

	void Initialize();
	void Update();
	void Draw(UINT rootParameterIndex);

	void TransferBuffer();

	void SetLightDirection(Vector3 lightdir);
	void SetLightColor(Vector3 color);

	bool active = false;

private://変数
	//定数バッファ
	ConstBuffer<LightConstBufferData> constBuff;

	bool dirty = false;
};