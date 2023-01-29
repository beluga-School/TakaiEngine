#include "Obj.h"
#include "Result.h"

LightGroup* Obj3d::lightGroup = nullptr;
PipeLineMode Obj3d::mode = PipeLineMode::PHONG;

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
	model = &ModelManager::GetInstance()->cubeM;
	texture = &TextureManager::GetInstance()->def;
}

void Obj3d::Update(Camera& camera)
{
	Matrix4 matScale;	//スケーリング行列
	Matrix4 matRot;	//回転行列
	Matrix4 matTrans;	//平行移動行列

	//matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	//スケールを設定
	matScale = Matrix4::Identity();
	matScale = Matrix4::scale(scale);

	//回転を設定
	matRot = Matrix4::Identity();
	matRot = Matrix4::rotateZ(rotation.z) * Matrix4::rotateX(rotation.x) * Matrix4::rotateY(rotation.y);

	//matRot = XMMatrixIdentity();
	//matRot *= XMMatrixRotationZ(rotation.z);
	//matRot *= XMMatrixRotationX(rotation.x);
	//matRot *= XMMatrixRotationY(rotation.y);

	matTrans = Matrix4::Identity();
	matTrans = Matrix4::translate(position);
	//matTrans = XMMatrixTranslation(position.x, position.y, position.z);

	matWorld = Matrix4::Identity();
	matWorld = matScale * matRot * matTrans;

	if (parent != nullptr)
	{	
		if (notScaleFlag)
		{
			/*Matrix4 matRotPar;
			Matrix4 matTransPar;

			matRotPar *= XMMatrixRotationZ(parent->rotation.z);
			matRotPar *= XMMatrixRotationX(parent->rotation.x);
			matRotPar *= XMMatrixRotationY(parent->rotation.y);

			matTransPar = XMMatrixTranslation(
				parent->position.x, parent->position.y, parent->position.z);

			matWorld *= matRotPar;
			matWorld *= matTransPar;*/
		}
		else
		{
			matWorld = matWorld * parent->matWorld;
		}

	}

	lightGroup->Update();

	//constBufferT.constBufferData->mat = matWorld * matView * matProjection;
	constBufferT.constBufferData->viewproj = camera.matView * camera.matProjection;
	constBufferT.constBufferData->world = matWorld;
	constBufferT.constBufferData->cameraPos = camera.eye;

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
		matWorld.m[3][0],
		matWorld.m[3][1],
		matWorld.m[3][2]
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

	if (mode == PipeLineMode::PHONG)
	{
		lightGroup->Draw(4);
	}

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

	if (mode == PipeLineMode::PHONG)
	{
		lightGroup->Draw(4);
	}

	//描画コマンド
	dx12->commandList->DrawIndexedInstanced(model->indices.size(), 1, 0, 0, 0);
}