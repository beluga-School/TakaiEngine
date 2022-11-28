#include "Billboard.h"
#include "DirectXInit.h"

Billboard::Billboard(Camera* view, bool yBillboardMode)
{
	this->view = view;
	this->yBillboardMode = yBillboardMode;
}

void Billboard::Update(XMMATRIX& matProjection)
{
	XMMATRIX matScale;	//�X�P�[�����O�s��
	XMMATRIX matRot;	//��]�s��
	XMMATRIX matTrans;	//���s�ړ��s��

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
	////	 �� �s��͂Ȃ�Ɗ|���Z�ɂ����1�ɂ܂Ƃ߂邱�Ƃ��ł����ł��I�I�I�I
	////		�s��͊|���鏇�Ԃɂ���Č��ʂ��ς��̂Œ��ӁI�I�I���ӁI�I�I���ӁI�I�I

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
	
	//SRV�q�[�v�̐擪���珇�Ԃ�SRV�����[�g�p�����[�^1�Ԃɐݒ�
	//���[�g�p�����[�^1�Ԃ̓e�N�X�`���o�b�t�@
	dx12->commandList->SetGraphicsRootDescriptorTable(1, texture->gpuHandle);
	//commandList->SetGraphicsRootConstantBufferView(0, constBufferT.buffer->GetGPUVirtualAddress());

	//���_�o�b�t�@�̐ݒ�
	dx12->commandList->IASetVertexBuffers(0, 1, &model->vbView);

	//�C���f�b�N�X�o�b�t�@�̐ݒ�
	dx12->commandList->IASetIndexBuffer(&model->ibView);

	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	dx12->commandList->SetGraphicsRootConstantBufferView(0, constBufferMaterial.buffer->GetGPUVirtualAddress());
	//commandList->SetGraphicsRootConstantBufferView(0, constBufferM.buffer->GetGPUVirtualAddress());

	dx12->commandList->SetGraphicsRootConstantBufferView(2, constBufferT.buffer->GetGPUVirtualAddress());
	
	dx12->commandList->SetGraphicsRootConstantBufferView(3, constBufferB.buffer->GetGPUVirtualAddress());

	//�`��R�}���h
	dx12->commandList->DrawIndexedInstanced(model->indices.size(), 1, 0, 0, 0);
}
