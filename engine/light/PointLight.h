#pragma once
#include "Vector3.h"

class PointLight
{
public://定数バッファに送るデータ
	struct ConstBufferData
	{
		Vector3 lightPos;
		float pad1;
		Vector3 lightColor;
		float pad2;
		Vector3 lighttAtten;
		bool active;
	};
public://メンバ変数
	//ライト座標
	Vector3 mLightPos = {0,0,0};
	//ライトの色
	Vector3 mLightColor = { 1,1,1 };
	//ライト距離減衰係数
	Vector3 mLightAtten = { 1.0f,1.0f,1.0f };
	
	bool mActive = false;
};
