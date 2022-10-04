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
	XMMATRIX matScale;	//�X�P�[�����O�s��
	XMMATRIX matRot;	//��]�s��
	XMMATRIX matTrans;	//���s�ړ��s��

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

	constBufferT.constBufferData->mat = matWorld * matView * matProjection;
	constBufferM.constBufferData->color = XMFLOAT4(1, 1, 1, 1.0f);
	////	 �� �s��͂Ȃ�Ɗ|���Z�ɂ����1�ɂ܂Ƃ߂邱�Ƃ��ł����ł��I�I�I�I
	////		�s��͊|���鏇�Ԃɂ���Č��ʂ��ς��̂Œ��ӁI�I�I���ӁI�I�I���ӁI�I�I
}

void Obj3d::Draw(ID3D12GraphicsCommandList* commandList,Texture *texture) {
	//SRV�q�[�v�̐ݒ�R�}���h
	commandList->SetDescriptorHeaps(1, &texture->texData.srvHeap);
	//SRV�q�[�v�̐擪���珇�Ԃ�SRV�����[�g�p�����[�^1�Ԃɐݒ�
	//���[�g�p�����[�^1�Ԃ̓e�N�X�`���o�b�t�@
	commandList->SetGraphicsRootDescriptorTable(1, texture->GetHandle());
	//commandList->SetGraphicsRootConstantBufferView(0, constBufferT.buffer->GetGPUVirtualAddress());

	//���_�o�b�t�@�̐ݒ�
	commandList->IASetVertexBuffers(0, 1, &model->vbView);

	//�C���f�b�N�X�o�b�t�@�̐ݒ�
	commandList->IASetIndexBuffer(&model->ibView);
	
	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	commandList->SetGraphicsRootConstantBufferView(0, constBufferM.buffer->GetGPUVirtualAddress());

	commandList->SetGraphicsRootConstantBufferView(2, constBufferT.buffer->GetGPUVirtualAddress());

	//�`��R�}���h
	commandList->DrawIndexedInstanced(model->indices.size() , 1, 0, 0, 0);
}