#include "Obj.h"
#include "Result.h"

void Obj3d::SetModel(Model* model)
{
	this->model = model;
}

void Obj3d::SetTexture(Texture* texture)
{
	this->texture = texture;
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
		if (notScaleFlag)
		{
			XMMATRIX matRotPar;
			XMMATRIX matTransPar;

			matRotPar = XMMatrixIdentity();
			matRotPar *= XMMatrixRotationZ(parent->rotation.z);
			matRotPar *= XMMatrixRotationX(parent->rotation.x);
			matRotPar *= XMMatrixRotationY(parent->rotation.y);

			matTransPar = XMMatrixTranslation(
				parent->position.x, parent->position.y, parent->position.z);

			matWorld *= matRotPar;
			matWorld *= matTransPar;
		}
		else
		{
			matWorld *= parent->matWorld;
		}

	}

	constBufferT.constBufferData->mat = matWorld * matView * matProjection;
	////	 ↑ 行列はなんと掛け算によって1つにまとめることができるんです！！！！
	////		行列は掛ける順番によって結果が変わるので注意！！！注意！！！注意！！！

	//constBufferM.constBufferData->color = XMFLOAT4(1, 1, 1, 1.0f);

	//ConstBufferDataB1* constMap1 = nullptr;
	//result = constBufferMaterial.buffer->Map(0, nullptr, (void**)&constMap1);
	constBufferMaterial.constBufferData->ambient = model->material.ambient;
	constBufferMaterial.constBufferData->diffuse = model->material.diffuse;
	constBufferMaterial.constBufferData->specular = model->material.specular;
	constBufferMaterial.constBufferData->alpha = model->material.alpha;
	constBufferB.constBufferData->brightness = color;
	//constBufferMaterial.buffer->Unmap(0, nullptr);
}

Vector3 Obj3d::GetWorldTrans()
{
	Vector3 worldTrans={
		matWorld.r[3].m128_f32[0],
		matWorld.r[3].m128_f32[1],
		matWorld.r[3].m128_f32[2]
	};
	return worldTrans;
}

void Obj3d::Draw() {
	DirectX12* dx12 = DirectX12::GetInstance();
	TextureManager* texM = TextureManager::GetInstance();
	
	//SRVヒープの先頭から順番にSRVをルートパラメータ1番に設定
	//ルートパラメータ1番はテクスチャバッファ
	dx12->commandList->SetGraphicsRootDescriptorTable(1, texture->gpuHandle);
	//commandList->SetGraphicsRootConstantBufferView(0, constBufferT.buffer->GetGPUVirtualAddress());

	//頂点バッファの設定
	dx12->commandList->IASetVertexBuffers(0, 1, &model->vbView);

	//インデックスバッファの設定
	dx12->commandList->IASetIndexBuffer(&model->ibView);
	
	//定数バッファビュー(CBV)の設定コマンド
	dx12->commandList->SetGraphicsRootConstantBufferView(0, constBufferMaterial.buffer->GetGPUVirtualAddress());
	//commandList->SetGraphicsRootConstantBufferView(0, constBufferM.buffer->GetGPUVirtualAddress());

	dx12->commandList->SetGraphicsRootConstantBufferView(2, constBufferT.buffer->GetGPUVirtualAddress());
	
	dx12->commandList->SetGraphicsRootConstantBufferView(3, constBufferB.buffer->GetGPUVirtualAddress());

	//描画コマンド
	dx12->commandList->DrawIndexedInstanced(model->indices.size() , 1, 0, 0, 0);
}

void Obj3d::DrawMaterial() {
	DirectX12* dx12 = DirectX12::GetInstance();
	TextureManager* texM = TextureManager::GetInstance();

	//SRVヒープの先頭から順番にSRVをルートパラメータ1番に設定
	//ルートパラメータ1番はテクスチャバッファ
	dx12->commandList->SetGraphicsRootDescriptorTable(1, model->material.tex->gpuHandle);
	//commandList->SetGraphicsRootConstantBufferView(0, constBufferT.buffer->GetGPUVirtualAddress());

	//頂点バッファの設定
	dx12->commandList->IASetVertexBuffers(0, 1, &model->vbView);

	//インデックスバッファの設定
	dx12->commandList->IASetIndexBuffer(&model->ibView);

	//定数バッファビュー(CBV)の設定コマンド
	dx12->commandList->SetGraphicsRootConstantBufferView(0, constBufferMaterial.buffer->GetGPUVirtualAddress());

	dx12->commandList->SetGraphicsRootConstantBufferView(2, constBufferT.buffer->GetGPUVirtualAddress());

	dx12->commandList->SetGraphicsRootConstantBufferView(3, constBufferB.buffer->GetGPUVirtualAddress());

	//描画コマンド
	dx12->commandList->DrawIndexedInstanced(model->indices.size(), 1, 0, 0, 0);
}