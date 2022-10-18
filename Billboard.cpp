#include "Billboard.h"
#include "DirectXInit.h"

Billboard::Billboard(View* view)
{
	this->view = view;
}


void Billboard::Update(XMMATRIX& matProjection)
{

	XMMATRIX matScale;	//�X�P�[�����O�s��
	XMMATRIX matRot;	//��]�s��
	XMMATRIX matTrans;	//���s�ړ��s��

	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);

	matRot = XMMatrixIdentity();

	//�����I�I�I
	XMVECTOR cameraAxisZ = {
	view->target.x - view->eye.x,
	view->target.y - view->eye.y,
	view->target.z - view->eye.z,
	};

	XMVECTOR viewUpVec = {
		view->up.x,
		view->up.y,
		view->up.z
	};

	XMVECTOR viewEye = {
	view->eye.x,
	view->eye.y,
	view->eye.z
	};

	XMVECTOR viewTarget = {
view->target.x,
view->target.y,
view->target.z
	};

	cameraAxisZ = XMVector3Normalize(cameraAxisZ);

	XMVECTOR cameraAxisX = XMVector3Cross(viewUpVec, cameraAxisZ);
	cameraAxisX = XMVector3Normalize(cameraAxisX);

	XMVECTOR cameraAxisY;
	cameraAxisY = XMVector3Cross(cameraAxisZ, cameraAxisX);

	XMMATRIX matCameraRot;
	matCameraRot.r[0] = cameraAxisX;
	matCameraRot.r[1] = cameraAxisY;
	matCameraRot.r[2] = cameraAxisZ;
	matCameraRot.r[3] = XMVectorSet(0, 0, 0,1);

	view->matView = XMMatrixTranspose(matCameraRot);

	XMVECTOR reverseEyePosition = XMVectorNegate(viewEye);

	XMVECTOR tX = XMVector3Dot(view->matView.r[0], reverseEyePosition);
	XMVECTOR tY = XMVector3Dot(view->matView.r[1], reverseEyePosition);
	XMVECTOR tZ = XMVector3Dot(view->matView.r[2], reverseEyePosition);

	XMVECTOR translation = XMVectorSet(
		tX.m128_f32[0],
		tY.m128_f32[1],
		tZ.m128_f32[2],
		1.0f);
	view->matView.r[3] = translation;

	matTrans = XMMatrixTranslation(
		position.x, position.y, position.z);

	XMMATRIX matBillboard = XMMatrixIdentity();
	matBillboard.r[0] = cameraAxisX;
	matBillboard.r[1] = cameraAxisY;
	matBillboard.r[2] = cameraAxisZ;
	matBillboard.r[3] = XMVectorSet(0, 0, 0, 1);

	matWorld = XMMatrixIdentity();

	matWorld *= matBillboard;

	matWorld *= matScale;
	matWorld *= matRot;
	matWorld *= matTrans;

	if (parent != nullptr)
	{
		matWorld *= parent->matWorld;
	}

	constBufferT.constBufferData->mat = matWorld * view->matView * matProjection;

	constBufferM.constBufferData->color = XMFLOAT4(1, 1, 1, 1.0f);

	ConstBufferDataB1* constMap1 = nullptr;
	result = constBufferMaterial.buffer->Map(0, nullptr, (void**)&constMap1);
	constMap1->ambient = model->material.ambient;
	constMap1->diffuse = model->material.diffuse;
	constMap1->specular = model->material.specular;
	constMap1->alpha = model->material.alpha;
	constBufferMaterial.buffer->Unmap(0, nullptr);

}

void Billboard::Draw(Texture* texture)
{
	DirectX12 *dx12 = DirectX12::GetInstance();
	//SRV�q�[�v�̐ݒ�R�}���h
	dx12->commandList->SetDescriptorHeaps(1, &texture->texData.srvHeap);
	//SRV�q�[�v�̐擪���珇�Ԃ�SRV�����[�g�p�����[�^1�Ԃɐݒ�
	//���[�g�p�����[�^1�Ԃ̓e�N�X�`���o�b�t�@
	dx12->commandList->SetGraphicsRootDescriptorTable(1, texture->GetHandle());
	//commandList->SetGraphicsRootConstantBufferView(0, constBufferT.buffer->GetGPUVirtualAddress());

	//���_�o�b�t�@�̐ݒ�
	dx12->commandList->IASetVertexBuffers(0, 1, &model->vbView);

	//�C���f�b�N�X�o�b�t�@�̐ݒ�
	dx12->commandList->IASetIndexBuffer(&model->ibView);

	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	dx12->commandList->SetGraphicsRootConstantBufferView(0, constBufferMaterial.buffer->GetGPUVirtualAddress());
	//commandList->SetGraphicsRootConstantBufferView(0, constBufferM.buffer->GetGPUVirtualAddress());

	dx12->commandList->SetGraphicsRootConstantBufferView(2, constBufferT.buffer->GetGPUVirtualAddress());

	//�`��R�}���h
	dx12->commandList->DrawIndexedInstanced(model->indices.size(), 1, 0, 0, 0);
}
