#include "LightGroup.h"
#include "DirectXInit.h"

LightGroup* LightGroup::Create()
{
	LightGroup* instance = new LightGroup();
	instance->Initialize();

	return instance;
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

void LightGroup::Draw(UINT index)
{
	DirectX12::GetInstance()->commandList->SetGraphicsRootConstantBufferView(index,
		constBuff.buffer->GetGPUVirtualAddress());
}

void LightGroup::TransferBuffer()
{
	for (int i = 0; i < DirLightNum; i++)
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
}

void LightGroup::SetAmbientColor(Vector3& color)
{
	ambienColor = color;
	dirty = true;
}

void LightGroup::SetDirLightActive(int index, bool active)
{
	assert(0 <= index && index < DirLightNum);
	dirLights[index].active = active;
}

void LightGroup::SetDirLightDir(int index, Vector3 lightdir)
{
	assert(0 <= index && index < DirLightNum);
	dirLights[index].direction = lightdir;
	dirty = true;
}

void LightGroup::SetDirLightColor(int index, Vector3 lightcolor)
{
	assert(0 <= index && index < DirLightNum);
	dirLights[index].color = lightcolor;
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
