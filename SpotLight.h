#pragma once
#include "Vector3.h"
#include "Float4.h"
#include "Vector2.h"

class CircleShadow
{
public:
	struct ConstBufferData
	{
		Vector3 direction;
		float distance;
		Vector3 casterPos;
		bool active;
		Vector3 atten;
		float pad2;
		Vector2 factorAngleCos;
	};
public://メンバ変数
	Vector3 mDirection = { 0.0f,1.0f,0.0f };

	float distance = 1.0f;

	Vector3 casterPos = { 0,0,0 };

	Vector3 atten = { 0.5f,0.6f,0.0f };

	Vector2 factorAngleCos = { 0.9f,0.85f };

	bool mActive = false;
};

