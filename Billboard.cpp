#include "Billboard.h"
#include "DirectXInit.h"

Billboard::Billboard(Camera* view, bool yBillboardMode)
{
	this->view = view;
	this->yBillboardMode = yBillboardMode;
}

void Billboard::Update(XMMATRIX& matProjection)
{
	XMMATRIX matScale;	//スケーリング行列
	XMMATRIX matRot;	//回転行列
	XMMATRIX matTrans;	//平行移動行列

	XMMATRIX reverseMatView;
	reverseMatView = XMMatrixInverse(nullptr, view->matView);
	reverseMatView.r[3] = XMVectorSet(0, 0, 0, 1);

	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(rotation.z);
	matRot *= XMMatrixRotationX(rotation.x);
	matRot *= XMMatrixRotationY(rotation.y);
	matTrans = XMMatrixTranslation(
		position.x, position.y, position.z);

	matBillboardY = XMMatrixIdentity();
	XMVECTOR upVector = { 0,1,0 };
	XMVECTOR cameraAxisX = {
		view->matView.r[0].m128_f32[0],
		view->matView.r[1].m128_f32[0],
		view->matView.r[2].m128_f32[0],
	};

	XMVECTOR axisZ = XMVector3Cross(cameraAxisX, upVector);

	matBillboardY.r[0] = cameraAxisX;	//x
	matBillboardY.r[2] = axisZ;			//z

	matBillboardY.r[1] = XMVector3Cross(axisZ,cameraAxisX);	//y
	
	matBillboardY.r[3] = XMVectorSet(0, 0, 0, 1);

	matWorld = XMMatrixIdentity();
	matWorld *= matScale;
	matWorld *= matRot;
	matWorld *= matTrans;

	if (parent != nullptr)
	{
		matWorld *= parent->matWorld;
	}

	if (yBillboardMode)
	{
		constBufferT.constBufferData->mat = matWorld * matBillboardY * view->matView * matProjection;
	}
	else
	{
		constBufferT.constBufferData->mat = matWorld * reverseMatView * view->matView * matProjection;
	}
	////	 ↑ 行列はなんと掛け算によって1つにまとめることができるんです！！！！
	////		行列は掛ける順番によって結果が変わるので注意！！！注意！！！注意！！！

	//constBufferM.constBufferData->color = XMFLOAT4(1, 1, 1, 1.0f);

	constBufferMaterial.constBufferData->ambient = model->material.ambient;
	constBufferMaterial.constBufferData->diffuse = model->material.diffuse;
	constBufferMaterial.constBufferData->specular = model->material.specular;
	constBufferMaterial.constBufferData->alpha = model->material.alpha;
	constBufferB.constBufferData->brightness = color;
}


void Billboard::Draw()
{
	DirectX12 *dx12 = DirectX12::GetInstance();
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
	dx12->commandList->DrawIndexedInstanced(model->indices.size(), 1, 0, 0, 0);
}
