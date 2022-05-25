#pragma once
#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

class Obj
{
public:

	XMMATRIX matWorld;	//ワールド変換行列
	XMMATRIX matScale;	//スケーリング行列
	XMMATRIX matRot;	//回転行列
	XMMATRIX matTrans;	//平行移動行列

public:
	void MatWorldIdentity();
	void MatScaleIdentity();
	void MatRotIdentity();
	void MatTransIdentity();

	void ScaleUpdate(float matSx,float matSy,float matSz);
	void ScaleUpdate(XMFLOAT3 scale);

	void RotUpdateZXY(float matRx, float matRy, float matRz);
	void RotUpdateZXY(XMFLOAT3 rot);

	void TransUpdate(float posx, float posy, float posz);
	void TransUpdate(XMFLOAT3 pos);

	void MatWorldUpdate();

	XMMATRIX GetMatWorld();
};

