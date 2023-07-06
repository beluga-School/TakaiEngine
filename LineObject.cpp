#include "LineObject.h"

void LineObject::Initialize()
{
	MODEL = ModelManager::GetModel("LineSphere");
}

void LineObject::Update(const Camera& camera)
{
	Matrix4 matScale;	//�X�P�[�����O�s��
	Matrix4 matRot;	//��]�s��
	Matrix4 matTrans;	//���s�ړ��s��

	//�X�P�[����ݒ�
	matScale = Matrix4::Identity();
	matScale = Matrix4::scale(scale);

	//��]��ݒ�
	matRot = Matrix4::Identity();
	matRot = Matrix4::rotateZ(rotation.z) * Matrix4::rotateX(rotation.x) * Matrix4::rotateY(rotation.y);

	matTrans = Matrix4::Identity();
	matTrans = Matrix4::translate(position);

	matWorld = Matrix4::Identity();
	matWorld = matScale * matRot * matTrans;

	if (parent != nullptr)
	{
		matWorld = matWorld * parent->matWorld;
	}

	constBufferLine.mConstBufferData->world = matWorld;
	constBufferLine.mConstBufferData->viewproj = camera.mMatView * camera.mMatProjection;
	constBufferLine.mConstBufferData->color = {
		color_.x,color_.y,color_.z,color_.w
	};
}

void LineObject::Draw()
{
	//�����Ȃ��t���O�������Ă�Ȃ�`����s��Ȃ�
	if (mIsVisiable == false)
	{
		return;
	}

	DirectX12* dx12 = DirectX12::Get();

	//���_�o�b�t�@�̐ݒ�
	dx12->mCmdList->IASetVertexBuffers(0, 1, &MODEL->mVbView);

	//�C���f�b�N�X�o�b�t�@�̐ݒ�
	dx12->mCmdList->IASetIndexBuffer(&MODEL->mIbView);

	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	dx12->mCmdList->SetGraphicsRootConstantBufferView(0, constBufferLine.mBuffer->GetGPUVirtualAddress());

	//�`��R�}���h
	dx12->mCmdList->DrawIndexedInstanced((UINT)MODEL->mMesh.indices.size(), 1, 0, 0, 0);
}
