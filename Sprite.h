#pragma once
#include "Vector3.h"
#include "Vector2.h"

struct VertexPosUV
{
	Vector3 pos;
	Vector2 uv;
};

class Sprite
{
public:
	void Initialize();
	void Update();
	void Draw();
};

