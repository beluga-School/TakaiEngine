#include "Billboard.h"
#include "DirectXInit.h"
#include "PlayerCamera.h"

Billboard::Billboard()
{
	Initialize();
}

void Billboard::Initialize()
{
	SetModel(ModelManager::GetModel("plate"));
	SetTexture(TextureManager::GetTexture("default"));
}

void Billboard::Update(const Camera& camera)
{
	Matrix4 MatBillboard = Matrix4::Identity();
	
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

	//�r���{�[�h�s����쐬
	MatBillboard = 
		Matrix4::rotateZ(PlayerCamera::Get()->rotation.z) *
		Matrix4::rotateX(PlayerCamera::Get()->rotation.x) * 
		Matrix4::rotateY(PlayerCamera::Get()->rotation.y);

	matWorld = Matrix4::Identity();
	matWorld *= matScale;
	matWorld *= MatBillboard;//�r���{�[�h��]
	matWorld *= matRot;
	matWorld *= matTrans;

	constBufferT.mConstBufferData->viewproj = camera.mMatView * camera.mMatProjection;
	constBufferT.mConstBufferData->world = matWorld;
	constBufferT.mConstBufferData->cameraPos = camera.mEye;
	constBufferT.mConstBufferData->tiling = mTiling;

	if (MODEL != nullptr)
	{
		constBufferB1.mConstBufferData->ambient = MODEL->mMaterial.mAmbient;
		constBufferB1.mConstBufferData->diffuse = MODEL->mMaterial.mDiffuse;
		constBufferB1.mConstBufferData->specular = MODEL->mMaterial.mSpecular;
		constBufferB1.mConstBufferData->alpha = MODEL->mMaterial.mAlpha;
		constBufferB.mConstBufferData->brightness = color_;
	}

	constBufferOutLine.mConstBufferData->color = mOutLineColor;
	constBufferOutLine.mConstBufferData->thickness = mOutLineThickness;

	transparentTimer.Update();
}

BillboardY::BillboardY()
{
	Initialize();
}

void BillboardY::Initialize()
{
	SetModel(ModelManager::GetModel("plate"));
	SetTexture(TextureManager::GetTexture("default"));
}

void BillboardY::Update(const Camera& camera)
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

	//�r���{�[�h�s����쐬
	Matrix4 MatBillboardY = Matrix4::Identity();
	Vector3 camVecX = PlayerCamera::Get()->matWorld.ExtractAxisX();
	MatBillboardY.m[0][0] = camVecX.x;
	MatBillboardY.m[0][1] = camVecX.y;
	MatBillboardY.m[0][2] = camVecX.z;
	
	Vector3 upVec = { 0,1,0 };
	MatBillboardY.m[1][0] = upVec.x;
	MatBillboardY.m[1][1] = upVec.y;
	MatBillboardY.m[1][2] = upVec.z;

	Vector3 camVecZ = PlayerCamera::Get()->matWorld.ExtractAxisX().GetCross(upVec);
	MatBillboardY.m[2][0] = camVecZ.x;
	MatBillboardY.m[2][1] = camVecZ.y;
	MatBillboardY.m[2][2] = camVecZ.z;

	matWorld = Matrix4::Identity();
	matWorld *= matScale;
	matWorld *= MatBillboardY;//�r���{�[�h��]
	matWorld *= matRot;
	matWorld *= matTrans;

	constBufferT.mConstBufferData->viewproj = camera.mMatView * camera.mMatProjection;
	constBufferT.mConstBufferData->world = matWorld;
	constBufferT.mConstBufferData->cameraPos = camera.mEye;
	constBufferT.mConstBufferData->tiling = mTiling;

	if (MODEL != nullptr)
	{
		constBufferB1.mConstBufferData->ambient = MODEL->mMaterial.mAmbient;
		constBufferB1.mConstBufferData->diffuse = MODEL->mMaterial.mDiffuse;
		constBufferB1.mConstBufferData->specular = MODEL->mMaterial.mSpecular;
		constBufferB1.mConstBufferData->alpha = MODEL->mMaterial.mAlpha;
		constBufferB.mConstBufferData->brightness = color_;
	}

	constBufferOutLine.mConstBufferData->color = mOutLineColor;
	constBufferOutLine.mConstBufferData->thickness = mOutLineThickness;

	//���͒��_�V�F�[�_�[�Ƃ��͋��ʉ��������������񂶂�Ȃ���
	cBufferNoise.mConstBufferData->viewproj = camera.mMatView * camera.mMatProjection;
	cBufferNoise.mConstBufferData->matWorld = matWorld;

	transparentTimer.Update();
}

void BillboardY::DrawNoise()
{
	//�����Ȃ��t���O�������Ă�Ȃ�`����s��Ȃ�
	if (mIsVisiable == false)
	{
		return;
	}

	DirectX12* dx12 = DirectX12::Get();
	TextureManager* texM = TextureManager::Get();

	//SRV�q�[�v�̐擪���珇�Ԃ�SRV�����[�g�p�����[�^1�Ԃɐݒ�
	//���[�g�p�����[�^1�Ԃ̓e�N�X�`���o�b�t�@
	//dx12->mCmdList->SetGraphicsRootDescriptorTable(1, TEXTURE->mGpuHandle);
	//commandList->SetGraphicsRootConstantBufferView(0, constBufferT.buffer->GetGPUVirtualAddress());

	//���_�o�b�t�@�̐ݒ�
	dx12->mCmdList->IASetVertexBuffers(0, 1, &MODEL->mVbView);

	//�C���f�b�N�X�o�b�t�@�̐ݒ�
	dx12->mCmdList->IASetIndexBuffer(&MODEL->mIbView);

	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	dx12->mCmdList->SetGraphicsRootConstantBufferView(0, constBufferB1.mBuffer->GetGPUVirtualAddress());
	
	//�`��R�}���h
	dx12->mCmdList->DrawIndexedInstanced((UINT)MODEL->mMesh.indices.size(), 1, 0, 0, 0);

}