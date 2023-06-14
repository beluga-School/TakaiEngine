#include "LightGroup.h"
#include "DirectXInit.h"

std::unique_ptr<LightGroup> LightGroup::lightGroup = nullptr;

void LightGroup::Create()
{
	lightGroup = std::make_unique<LightGroup>();
	lightGroup->Initialize();
}

void LightGroup::Initialize()
{
	//トラブル防止のデフォライト
	DefaultLightSet();

	TransferBuffer();
}

void LightGroup::Update()
{
	if (dirty)
	{
		TransferBuffer();
		dirty = false;
	}
}

void LightGroup::Draw(const UINT& index)
{
	DirectX12::Get()->commandList->SetGraphicsRootConstantBufferView(index,
		constBuff.buffer->GetGPUVirtualAddress());
}

void LightGroup::TransferBuffer()
{
	for (int32_t i = 0; i < DirLightNum; i++)
	{
		//アクティブなら
		if (dirLights[i].active)
		{
			//定数バッファの設定を転送
			constBuff.constBufferData->dirLights[i].active = true;
			constBuff.constBufferData->dirLights[i].direction = -dirLights[i].direction;
			constBuff.constBufferData->dirLights[i].color = dirLights[i].color;
			constBuff.constBufferData->ambienColor = ambienColor;
		}
		else
		{
			constBuff.constBufferData->dirLights[i].active = false;
		}
	}
	for (int32_t i = 0; i < PointLightNum; i++)
	{
		if (pointLights[i].active) {
			constBuff.constBufferData->pointLights[i].active = true;
			constBuff.constBufferData->pointLights[i].lightPos = pointLights[i].lightPos;
			constBuff.constBufferData->pointLights[i].lightColor = pointLights[i].lightColor;
			constBuff.constBufferData->pointLights[i].lighttAtten = pointLights[i].lightAtten;
		}
		else
		{
			constBuff.constBufferData->pointLights[i].active = false;
		}
	}
}

void LightGroup::SetAmbientColor(const Vector3& color)
{
	ambienColor = color;
	dirty = true;
}

void LightGroup::SetDirLightActive(const int32_t& index, const bool& active)
{
	assert(0 <= index && index < DirLightNum);
	dirLights[index].active = active;
}

void LightGroup::SetDirLightDir(const int32_t& index, const Vector3& lightdir)
{
	assert(0 <= index && index < DirLightNum);
	dirLights[index].direction = lightdir;
	dirty = true;
}

void LightGroup::SetDirLightColor(const int32_t& index, const Vector3& lightcolor)
{
	assert(0 <= index && index < DirLightNum);
	dirLights[index].color = lightcolor;
	dirty = true;
}

void LightGroup::SetPointLightActive(const int32_t& index, const bool& active)
{
	assert(0 <= index && index < PointLightNum);

	pointLights[index].active = active;
}

void LightGroup::SetPointLightPos(const int32_t& index, const Vector3& pos)
{
	assert(0 <= index && index < PointLightNum);

	pointLights[index].lightPos = pos;
	dirty = true;
}

void LightGroup::SetPointLightColor(const int32_t& index, const Vector3& color)
{
	assert(0 <= index && index < PointLightNum);

	pointLights[index].lightColor = color;
	dirty = true;
}

void LightGroup::SetPointLightAtten(const int32_t& index, const Vector3& atten)
{
	assert(0 <= index && index < PointLightNum);

	pointLights[index].lightAtten = atten;
	dirty = true;
}

void LightGroup::DefaultLightSet()
{
	dirLights[0].active = true;
	dirLights[0].color = { 1.0f,0.0f,0.0f };
	dirLights[0].direction = { 0.0f,-1.0f,0.0f };
	
	dirLights[1].active = true;
	dirLights[1].color = { 0.0f,1.0f,0.0f };
	dirLights[1].direction = { 0.5f,0.1f,0.2f };
	
	dirLights[2].active = true;
	dirLights[2].color = { 0.0f,0.0f,1.0f };
	dirLights[2].direction = { -0.5f,0.1f,-0.2f };
}
