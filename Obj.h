#pragma once
#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")
#include "DirectXInit.h"
#include "Model.h"

struct ConstBufferDataMaterial {
	XMFLOAT4 color;	//色(RGBA)
};

struct ConstBufferDataTransform {
	XMMATRIX mat;	//3D変換行列
};

class Obj3d
{
public:
	ID3D12Resource* constBuffTransform;
	ConstBufferDataTransform* constMapTransform;

	XMFLOAT3 scale = { 1,1,1 };
	XMFLOAT3 rotation = { 0,0,0 };
	XMFLOAT3 position = { 0,0,0 };

	XMMATRIX matWorld;	//ワールド変換行列
	//XMMATRIX matScale;	//スケーリング行列
	//XMMATRIX matRot;	//回転行列
	//XMMATRIX matTrans;	//平行移動行列

	Obj3d* parent = nullptr;

	Model* model = nullptr;
public:
	void Initialize(ID3D12Device* device);
	void SetModel(Model *model);
	void Update(XMMATRIX& matView, XMMATRIX& matProjection);
	void Draw(ID3D12GraphicsCommandList* commandList);

	//void MatWorldIdentity();
	//void MatScaleIdentity();
	//void MatRotIdentity();
	//void MatTransIdentity();

	//void ScaleUpdate(float matSx,float matSy,float matSz);
	//void ScaleUpdate(XMFLOAT3 scale);

	//void RotUpdateZXY(float matRx, float matRy, float matRz);
	//void RotUpdateZXY(XMFLOAT3 rot);

	//void TransUpdate(float posx, float posy, float posz);
	//void TransUpdate(XMFLOAT3 pos);

	//void MatWorldUpdate();

	//XMMATRIX GetMatWorld();
};

