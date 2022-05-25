#include "Obj.h"

XMMATRIX Obj::GetMatWorld()
{
	return matWorld;
}

void Obj::ScaleUpdate(float matSx, float matSy, float matSz)
{
	matScale = XMMatrixScaling(matSx, matSy, matSz);
}

void Obj::ScaleUpdate(XMFLOAT3 scale)
{
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
}

void Obj::TransUpdate(float posx, float posy, float posz)
{
	matTrans = XMMatrixTranslation(posx, posy, posz);
}

void Obj::TransUpdate(XMFLOAT3 pos)
{
	matTrans = XMMatrixTranslation(pos.x, pos.y, pos.z);
}

void Obj::MatWorldUpdate()
{
	matWorld = XMMatrixIdentity();	//変形をリセット
	matWorld *= matScale;			//ワールド行列にスケーリングを反映
	matWorld *= matRot;				//ワールド行列に回転を反映
	matWorld *= matTrans;			//ワールド行列に平行移動を反映
}

void Obj::RotUpdateZXY(float matRx, float matRy, float matRz)
{
	matRot *= XMMatrixRotationZ(matRz);
	matRot *= XMMatrixRotationX(matRx);
	matRot *= XMMatrixRotationY(matRy);
}

void Obj::RotUpdateZXY(XMFLOAT3 rot)
{
	matRot *= XMMatrixRotationZ(rot.z);
	matRot *= XMMatrixRotationX(rot.x);
	matRot *= XMMatrixRotationY(rot.y);
}

void Obj::MatWorldIdentity()
{
	matWorld = XMMatrixIdentity();
}

void Obj::MatScaleIdentity()
{
	matScale = XMMatrixIdentity();
}

void Obj::MatRotIdentity()
{
	matRot = XMMatrixIdentity();
}

void Obj::MatTransIdentity()
{
	matTrans = XMMatrixIdentity();
}