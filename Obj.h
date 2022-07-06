#pragma once
#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")
#include "DirectXInit.h"
#include "Model.h"
#include "ConstBuffer.h"
#include "Texture.h"
#include <memory>

struct ConstBufferDataMaterial {
	XMFLOAT4 color;	//色(RGBA)
};

struct ConstBufferDataTransform {
	XMMATRIX mat;	//3D変換行列
};

class Obj3d
{
public:
	ConstBuffer<ConstBufferDataTransform> constBufferT;

	XMFLOAT3 scale = { 1,1,1 };
	XMFLOAT3 rotation = { 0,0,0 };
	XMFLOAT3 position = { 0,0,0 };

	XMMATRIX matWorld;	//ワールド変換行列
	//XMMATRIX matScale;	//スケーリング行列
	//XMMATRIX matRot;	//回転行列
	//XMMATRIX matTrans;	//平行移動行列

	std::unique_ptr<Obj3d> parent;

	Model* model = nullptr;
public:
	void Initialize();
	void SetModel(Model *model);
	void Update(XMMATRIX& matView, XMMATRIX& matProjection);
	void Draw(ID3D12GraphicsCommandList* commandList, Texture& texture);

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

