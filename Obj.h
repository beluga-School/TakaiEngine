#pragma once
#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")
#include "DirectXInit.h"
#include "Model.h"
#include "Texture.h"
#include <memory>
#include "Matrix4.h"

struct ConstBufferDataB1
{
	Vector3 ambient;	//アンビエント係数
	float pad1;			//パディング
	Vector3 diffuse;	//ディフューズ係数
	float pad2;			//パディング
	Vector3 specular;	//スペキュラー係数
	float alpha;
};

struct ConstBufferBrightness
{
	XMFLOAT4 brightness;		//カラー
};

class Obj3d
{
public:
	ConstBuffer<ConstBufferDataTransform> constBufferT;
	ConstBuffer<ConstBufferDataMaterial> constBufferM;
	ConstBuffer<ConstBufferBrightness> constBufferB;

	ConstBuffer<ConstBufferDataB1> constBufferMaterial;

	Vector3 scale = { 1.0f,1.0f,1.0f };
	Vector3 rotation = { 0,0,0 };
	Vector3 position = { 0,0,0 };

	XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f };

	Matrix4 matWorld;
	//XMMATRIX matWorld;	//ワールド変換行列

	bool notScaleFlag = false;

	Obj3d* parent = nullptr;
	Texture *texture = nullptr;

	Model* model = nullptr;
public:
	void Initialize();
	void SetModel(Model *model);
	void SetTexture(Texture *texture);
	void Update(Matrix4& matView, Matrix4& matProjection);

	Vector3 GetWorldTrans();

	void Draw();
	void DrawMaterial();

};

