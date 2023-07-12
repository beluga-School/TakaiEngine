#pragma once
#include "Obj.h"
#include "ViewProjection.h"
#include "Matrix4.h"

struct ConstBufferPerlinNoise
{
	Matrix4 viewproj;
	Matrix4 matWorld;
};

class Billboard : public Obj3d
{
public:
	Billboard();
	void Initialize();

	void Update(const Camera &camera);
};

class BillboardY : public Obj3d
{
public:
	BillboardY();
	void Initialize();

	void Update(const Camera& camera);

	void DrawNoise();

	ConstBuffer<ConstBufferPerlinNoise> cBufferNoise;
};