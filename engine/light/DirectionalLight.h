#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "ConstBuffer.h"

class DirectionalLight
{
public://変数
	Vector3 direction = { 1,0,0 };
	Vector3 color = { 1,1,1 };

	struct ConstBufferData
	{
		Vector3 direction;
		bool active;
		Vector3 color;
		float pad;
	};

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
	void Draw(const UINT& rootParameterIndex);

	void TransferBuffer();

	void SetLightDirection(const Vector3& lightdir);
	void SetLightColor(const Vector3& color);

	bool active = false;

private://変数
	//定数バッファ
	ConstBuffer<ConstBufferData> constBuff;

	bool dirty = false;
};