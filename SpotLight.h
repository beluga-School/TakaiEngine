#pragma once
#include "Vector3.h"
#include "Float4.h"

class SpotLight
{
public:
	struct ConstBufferData
	{
		Float4 lightColor;
		Vector3 lightPos;
		float intensity;
		Vector3 direction;
		float distance;
		float decay;
		float cosAngle;
		float cosFalloffStart;
		bool active;
	};
public://メンバ変数
	//ライト座標
	Vector3 mLightPos = { 0,0,0 };
	//ライトの色
	Vector3 mLightColor = { 1,1,1 };
	//ライト距離減衰係数
	Vector3 mLightAtten = { 1.0f,1.0f,1.0f };

	Vector3 mDirection = { 1.0f,0.0f,0.0f };

	float intensity = 0;
	float decay = 1.0f;
	float distance = 1.0f;
	float cosAngle = 0.0f;
	float cosFalloffStart = 0.0f;

	bool mActive = false;
};

