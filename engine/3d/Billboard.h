#pragma once
#include "Obj.h"
#include "Camera.h"
#include "Matrix4.h"

struct ConstBufferPerlinNoise
{
	Matrix4 viewproj;
	Matrix4 matWorld;
};

/*! Billboard
	ビルボード描画の3Dオブジェクト
*/
class Billboard : public Obj3d
{
public:
	Billboard();
	void Initialize();

	void Update(const Camera &camera);
};

/*! Billboard
	ビルボード描画の3Dオブジェクト(Y軸が正面を向く)
*/
class BillboardY : public Obj3d
{
public:
	BillboardY();
	void Initialize();

	void Update(const Camera& camera);

	//ノイズテクスチャを用いた描画
	void DrawNoise();

	ConstBuffer<ConstBufferPerlinNoise> cBufferNoise;
};