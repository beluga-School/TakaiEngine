#include "Obj.h"
#include "Result.h"

void Obj3d::SetModel(Model* model)
{
	this->model = model;
}

void Obj3d::Initialize()
{

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

	constBufferT.constBufferData->mat = matWorld * matView *matProjection;
	////	 ↑ 行列はなんと掛け算によって1つにまとめることができるんです！！！！
	////		行列は掛ける順番によって結果が変わるので注意！！！注意！！！注意！！！

	constBufferM.constBufferData->color = XMFLOAT4(1, 1, 1, 1.0f);

	ConstBufferDataB1* constMap1 = nullptr;
	result = constBufferMaterial.buffer->Map(0, nullptr, (void**)&constMap1);
	constMap1->ambient = model->material.ambient;
	constMap1->diffuse = model->material.diffuse;
	constMap1->specular = model->material.specular;
	constMap1->alpha = model->material.alpha;
	constBufferMaterial.buffer->Unmap(0, nullptr);
}

void Obj3d::Draw(ID3D12GraphicsCommandList* commandList,Texture *texture) {
	//SRVヒープの設定コマンド
	commandList->SetDescriptorHeaps(1, &texture->texData.srvHeap);
	//SRVヒープの先頭から順番にSRVをルートパラメータ1番に設定
	//ルートパラメータ1番はテクスチャバッファ
	commandList->SetGraphicsRootDescriptorTable(1, texture->GetHandle());
	//commandList->SetGraphicsRootConstantBufferView(0, constBufferT.buffer->GetGPUVirtualAddress());

	//頂点バッファの設定
	commandList->IASetVertexBuffers(0, 1, &model->vbView);

	//インデックスバッファの設定
	commandList->IASetIndexBuffer(&model->ibView);
	
	//定数バッファビュー(CBV)の設定コマンド
	commandList->SetGraphicsRootConstantBufferView(0, constBufferMaterial.buffer->GetGPUVirtualAddress());
	//commandList->SetGraphicsRootConstantBufferView(0, constBufferM.buffer->GetGPUVirtualAddress());

	commandList->SetGraphicsRootConstantBufferView(2, constBufferT.buffer->GetGPUVirtualAddress());

	//描画コマンド
	commandList->DrawIndexedInstanced(model->indices.size() , 1, 0, 0, 0);
}

void Obj3d::MaterialDraw(ID3D12GraphicsCommandList* commandList) {
	//SRVヒープの設定コマンド
	commandList->SetDescriptorHeaps(1, &model->material.tex->texData.srvHeap);
	//SRVヒープの先頭から順番にSRVをルートパラメータ1番に設定
	//ルートパラメータ1番はテクスチャバッファ
	commandList->SetGraphicsRootDescriptorTable(1, model->material.tex->GetHandle());
	//commandList->SetGraphicsRootConstantBufferView(0, constBufferT.buffer->GetGPUVirtualAddress());

	//頂点バッファの設定
	commandList->IASetVertexBuffers(0, 1, &model->vbView);

	//インデックスバッファの設定
	commandList->IASetIndexBuffer(&model->ibView);

	//定数バッファビュー(CBV)の設定コマンド
	commandList->SetGraphicsRootConstantBufferView(0, constBufferMaterial.buffer->GetGPUVirtualAddress());

	commandList->SetGraphicsRootConstantBufferView(2, constBufferT.buffer->GetGPUVirtualAddress());

	//描画コマンド
	commandList->DrawIndexedInstanced(model->indices.size(), 1, 0, 0, 0);
}