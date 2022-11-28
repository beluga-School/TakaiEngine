#pragma once
#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")
#include "DirectXInit.h"
#include "Model.h"
#include "Texture.h"
#include <memory>

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

	XMFLOAT3 scale = { 1.0f,1.0f,1.0f };
	XMFLOAT3 rotation = { 0,0,0 };
	XMFLOAT3 position = { 0,0,0 };

	XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f };

	XMMATRIX matWorld;	//ワールド変換行列
	//XMMATRIX matScale;	//スケーリング行列
	//XMMATRIX matRot;	//回転行列
	//XMMATRIX matTrans;	//平行移動行列

	bool notScaleFlag = false;

	Obj3d* parent = nullptr;
	Texture *texture = nullptr;

	Model* model = nullptr;
public:
	void Initialize();
	void SetModel(Model *model);
	void SetTexture(Texture *texture);
	void Update(XMMATRIX& matView, XMMATRIX& matProjection);

	Vector3 GetWorldTrans();

	void Draw();
	void DrawMaterial();
};

