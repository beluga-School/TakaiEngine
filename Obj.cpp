#include "Obj.h"
#include "Result.h"

void Obj3d::SetModel(Model* model)
{
	this->model = model;
}

void Obj3d::Initialize(ID3D12Device* device)
{
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	result = device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform));
	assert(SUCCEEDED(result));

	result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform);
	assert(SUCCEEDED(result));
}

void Obj3d::Update(XMMATRIX& matView, XMMATRIX& matProjection)
{
	XMMATRIX matScale;	//スケーリング行列
	XMMATRIX matRot;	//回転行列
	XMMATRIX matTrans;	//平行移動行列

	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(rotation.z);
	matRot *= XMMatrixRotationX(rotation.x);
	matRot *= XMMatrixRotationY(rotation.y);
	matTrans = XMMatrixTranslation(
		position.x, position.y, position.z);

	matWorld = XMMatrixIdentity();
	matWorld *= matScale;
	matWorld *= matRot;
	matWorld *= matTrans;

	if (parent != nullptr)
	{
		matWorld *= parent->matWorld;
	}

	constMapTransform->mat = matWorld * matView * matProjection;
	////	 ↑ 行列はなんと掛け算によって1つにまとめることができるんです！！！！
	////		行列は掛ける順番によって結果が変わるので注意！！！注意！！！注意！！！
}

void Obj3d::Draw(ID3D12GraphicsCommandList* commandList) {
	//頂点バッファの設定
	commandList->IASetVertexBuffers(0, 1, &model->vbView);
	//インデックスバッファの設定
	commandList->IASetIndexBuffer(&model->ibView);
	//定数バッファビュー(CBV)の設定コマンド
	commandList->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());

	//描画コマンド
	commandList->DrawIndexedInstanced(model->indices.size() , 1, 0, 0, 0);
}

//XMMATRIX Obj3d::GetMatWorld()
//{
//	return matWorld;
//}
//
//void Obj3d::ScaleUpdate(float matSx, float matSy, float matSz)
//{
//	matScale = XMMatrixScaling(matSx, matSy, matSz);
//}
//
//void Obj3d::ScaleUpdate(XMFLOAT3 scale)
//{
//	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
//}
//
//void Obj3d::TransUpdate(float posx, float posy, float posz)
//{
//	matTrans = XMMatrixTranslation(posx, posy, posz);
//}
//
//void Obj3d::TransUpdate(XMFLOAT3 pos)
//{
//	matTrans = XMMatrixTranslation(pos.x, pos.y, pos.z);
//}
//
//void Obj3d::MatWorldUpdate()
//{
//	matWorld = XMMatrixIdentity();	//変形をリセット
//	matWorld *= matScale;			//ワールド行列にスケーリングを反映
//	matWorld *= matRot;				//ワールド行列に回転を反映
//	matWorld *= matTrans;			//ワールド行列に平行移動を反映
//}
//
//void Obj3d::RotUpdateZXY(float matRx, float matRy, float matRz)
//{
//	matRot *= XMMatrixRotationZ(matRz);
//	matRot *= XMMatrixRotationX(matRx);
//	matRot *= XMMatrixRotationY(matRy);
//}
//
//void Obj3d::RotUpdateZXY(XMFLOAT3 rot)
//{
//	matRot *= XMMatrixRotationZ(rot.z);
//	matRot *= XMMatrixRotationX(rot.x);
//	matRot *= XMMatrixRotationY(rot.y);
//}
//
//void Obj3d::MatWorldIdentity()
//{
//	matWorld = XMMatrixIdentity();
//}
//
//void Obj3d::MatScaleIdentity()
//{
//	matScale = XMMatrixIdentity();
//}
//
//void Obj3d::MatRotIdentity()
//{
//	matRot = XMMatrixIdentity();
//}
//
//void Obj3d::MatTransIdentity()
//{
//	matTrans = XMMatrixIdentity();
//}