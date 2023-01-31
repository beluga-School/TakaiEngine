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

void DirectionalLight::Draw(UINT rootParameterIndex)
{
	DirectX12::GetInstance()->commandList->SetGraphicsRootConstantBufferView(
		rootParameterIndex, constBuff.buffer->GetGPUVirtualAddress()
	);
}

void DirectionalLight::TransferBuffer()
{
	constBuff.constBufferData->direction = -direction;
	constBuff.constBufferData->color = color;
}

void DirectionalLight::SetLightDirection(Vector3 lightdir)
{
	direction = lightdir;
	dirty = true;
}

void DirectionalLight::SetLightColor(Vector3 color)
{
	this->color = color;
	dirty = true;
}
