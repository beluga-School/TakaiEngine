#pragma once
#include "Vector3.h"
#include "Float4.h"

/*! PointLight
	点光源
*/

class PointLight
{
public:
	struct ConstBufferData
	{
		Float4 lightColor;
		Vector3 lightPos;
		float intensity;
	};
public://メンバ変数
	//ライト座標
	Vector3 mLightPos = {0,0,0};
	//ライトの色
	Vector3 mLightColor = { 1,1,1 };
	//ライト距離減衰係数
	Vector3 mLightAtten = { 1.0f,1.0f,1.0f };
	
	float intensity = 0;

	bool mActive = false;
};
