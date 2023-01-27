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

class Light
{
public:
	static Light* Create() {
		//3Dオブジェクトのインスタンスを生成
		Light* instance = new Light();
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

private:
	//定数バッファ
	ConstBuffer<LightConstBufferData> constBuff;

	Vector3 direction = { 1,0,0};
	Vector3 color = { 1,1,1 };

	bool dirty = false;
};