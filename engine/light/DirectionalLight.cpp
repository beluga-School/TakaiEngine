#include "DirectionalLight.h"
#include "DirectXInit.h"

void DirectionalLight::Initialize()
{
	TransferBuffer();
}

void DirectionalLight::Update()
{
	if (dirty) {
		TransferBuffer();
		dirty = false;
	}
}

void DirectionalLight::Draw(const UINT& rootParameterIndex)
{
	DirectX12::Get()->commandList->SetGraphicsRootConstantBufferView(
		rootParameterIndex, constBuff.buffer->GetGPUVirtualAddress()
	);
}

void DirectionalLight::TransferBuffer()
{
	constBuff.constBufferData->direction = -direction;
	constBuff.constBufferData->color = color;
}

void DirectionalLight::SetLightDirection(const Vector3& lightdir)
{
	direction = lightdir;
	dirty = true;
}

void DirectionalLight::SetLightColor(const Vector3& color)
{
	this->color = color;
	dirty = true;
}
