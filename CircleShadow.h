#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Float4.h"

/*
	丸影
*/
class CircleShadow
{
public:
	struct ConstBufferData
	{
		Vector3 casterPos;
		float distance;
		Vector3 atten;
		bool active;
		Vector3 direction;
		float pad1;
		Vector2 angleCos;
		float pad2[2];
	};

public:
	//方向
	Vector3 direction = {0,-1,0};

	//キャスターとライトの距離
	float distance = 0;
	
	//キャスターの座標
	Vector3 casterPos{};

	//距離減数の係数
	Vector3 atten = {0.5f,0.6f,0.0f};

	//減衰角度
	Vector2 factorAngleCos = { 0.2f,0.5f };

	//フラグ
	bool mActive = false;
};

