#include "Light.h"
#include "DirectXInit.h"

void Light::Initialize()
{
	TransferBuffer();
}

void Light::Update()
{
	if (dirty) {
		TransferBuffer();
		dirty = false;
	}
}

void Light::Draw(UINT rootParameterIndex)
{
	DirectX12::GetInstance()->commandList->SetGraphicsRootConstantBufferView(
		rootParameterIndex, constBuff.buffer->GetGPUVirtualAddress()
	);
}

void Light::TransferBuffer()
{
	constBuff.constBufferData->direction = -direction;
	constBuff.constBufferData->color = color;
}

void Light::SetLightDirection(Vector3 lightdir)
{
	direction = lightdir;
	dirty = true;
}

void Light::SetLightColor(Vector3 color)
{
	this->color = color;
	dirty = true;
}
