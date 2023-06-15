#include "Obj.h"
#include "Result.h"

void Obj3d::SetModel(const Model* model)
{
	this->model = model;
}

void Obj3d::SetTexture(const Texture* texture)
{
	this->texture = texture;
}

void Obj3d::Initialize()
{
	model = ModelManager::GetModel("Cube");
	texture = TextureManager::GetTexture("default");
}

void Obj3d::Update(const Camera& camera)
{
	Matrix4 matScale;	//�X�P�[�����O�s��
	Matrix4 matRot;	//��]�s��
	Matrix4 matTrans;	//���s�ړ��s��

	//matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	//�X�P�[����ݒ�
	matScale = Matrix4::Identity();
	matScale = Matrix4::scale(scale);

	//��]��ݒ�
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

	//constBufferT.constBufferData->mat = matWorld * matView * matProjection;
	constBufferT.mConstBufferData->viewproj = camera.mMatView * camera.mMatProjection;
	constBufferT.mConstBufferData->world = matWorld;
	constBufferT.mConstBufferData->cameraPos = camera.mEye;

	//constBufferM.constBufferData->color = XMFLOAT4(1, 1, 1, 1.0f);

	//ConstBufferDataB1* constMap1 = nullptr;
	//result = constBufferMaterial.buffer->Map(0, nullptr, (void**)&constMap1);
	constBufferMaterial.mConstBufferData->ambient = model->mMaterial.mAmbient;
	constBufferMaterial.mConstBufferData->diffuse = model->mMaterial.mDiffuse;
	constBufferMaterial.mConstBufferData->specular = model->mMaterial.mSpecular;
	constBufferMaterial.mConstBufferData->alpha = model->mMaterial.mAlpha;
	constBufferB.mConstBufferData->brightness = color_;
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
	DirectX12* dx12 = DirectX12::Get();
	TextureManager* texM = TextureManager::Get();
	
	//SRV�q�[�v�̐擪���珇�Ԃ�SRV�����[�g�p�����[�^1�Ԃɐݒ�
	//���[�g�p�����[�^1�Ԃ̓e�N�X�`���o�b�t�@
	dx12->mCmdList->SetGraphicsRootDescriptorTable(1, texture->mGpuHandle);
	//commandList->SetGraphicsRootConstantBufferView(0, constBufferT.buffer->GetGPUVirtualAddress());

	//���_�o�b�t�@�̐ݒ�
	dx12->mCmdList->IASetVertexBuffers(0, 1, &model->mVbView);

	//�C���f�b�N�X�o�b�t�@�̐ݒ�
	dx12->mCmdList->IASetIndexBuffer(&model->mIbView);
	
	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	dx12->mCmdList->SetGraphicsRootConstantBufferView(0, constBufferMaterial.mBuffer->GetGPUVirtualAddress());
	//commandList->SetGraphicsRootConstantBufferView(0, constBufferM.buffer->GetGPUVirtualAddress());

	dx12->mCmdList->SetGraphicsRootConstantBufferView(2, constBufferT.mBuffer->GetGPUVirtualAddress());
	
	dx12->mCmdList->SetGraphicsRootConstantBufferView(3, constBufferB.mBuffer->GetGPUVirtualAddress());

	//�`��R�}���h
	dx12->mCmdList->DrawIndexedInstanced((UINT)model->mMesh.indices.size() , 1, 0, 0, 0);
}

void Obj3d::DrawMaterial() {
	DirectX12* dx12 = DirectX12::Get();
	TextureManager* texM = TextureManager::Get();

	//SRV�q�[�v�̐擪���珇�Ԃ�SRV�����[�g�p�����[�^1�Ԃɐݒ�
	//���[�g�p�����[�^1�Ԃ̓e�N�X�`���o�b�t�@
	dx12->mCmdList->SetGraphicsRootDescriptorTable(1, model->mMaterial.mTextire->mGpuHandle);
	//commandList->SetGraphicsRootConstantBufferView(0, constBufferT.buffer->GetGPUVirtualAddress());

	//���_�o�b�t�@�̐ݒ�
	dx12->mCmdList->IASetVertexBuffers(0, 1, &model->mVbView);

	//�C���f�b�N�X�o�b�t�@�̐ݒ�
	dx12->mCmdList->IASetIndexBuffer(&model->mIbView);

	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	dx12->mCmdList->SetGraphicsRootConstantBufferView(0, constBufferMaterial.mBuffer->GetGPUVirtualAddress());

	dx12->mCmdList->SetGraphicsRootConstantBufferView(2, constBufferT.mBuffer->GetGPUVirtualAddress());

	dx12->mCmdList->SetGraphicsRootConstantBufferView(3, constBufferB.mBuffer->GetGPUVirtualAddress());

	//�`��R�}���h
	dx12->mCmdList->DrawIndexedInstanced((UINT)model->mMesh.indices.size(), 1, 0, 0, 0);
}