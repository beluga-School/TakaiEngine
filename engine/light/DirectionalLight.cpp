#include "DirectionalLight.h"
#include "DirectXInit.h"

void DirectionalLight::Initialize()
{
	TransferBuffer();
}

void DirectionalLight::Update()
{
	if (mDirty) {
		TransferBuffer();
		mDirty = false;
	}
}

void DirectionalLight::Draw(const UINT& rootParameterIndex)
{
	DirectX12::Get()->mCmdList->SetGraphicsRootConstantBufferView(
		rootParameterIndex, mConstBuff.mBuffer->GetGPUVirtualAddress()
	);
}

void DirectionalLight::TransferBuffer()
{
	mConstBuff.mConstBufferData->direction = -mDirection;
	mConstBuff.mConstBufferData->color = mColor;
}

void DirectionalLight::SetLightDirection(const Vector3& lightdir)
{
	mDirection = lightdir;
	mDirty = true;
}

void DirectionalLight::SetLightColor(const Vector3& color)
{
	this->mColor = color;
	mDirty = true;
}
