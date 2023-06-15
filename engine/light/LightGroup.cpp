#include "LightGroup.h"
#include "DirectXInit.h"

std::unique_ptr<LightGroup> LightGroup::sLightGroup = nullptr;

void LightGroup::Create()
{
	sLightGroup = std::make_unique<LightGroup>();
	sLightGroup->Initialize();
}

void LightGroup::Initialize()
{
	//トラブル防止のデフォライト
	DefaultLightSet();

	TransferBuffer();
}

void LightGroup::Update()
{
	if (mDirty)
	{
		TransferBuffer();
		mDirty = false;
	}
}

void LightGroup::Draw(const UINT& index)
{
	DirectX12::Get()->mCmdList->SetGraphicsRootConstantBufferView(index,
		mConstBuff.mBuffer->GetGPUVirtualAddress());
}

void LightGroup::TransferBuffer()
{
	for (int32_t i = 0; i < sDIRLIGHT_NUM; i++)
	{
		//アクティブなら
		if (mDirLights[i].mActive)
		{
			//定数バッファの設定を転送
			mConstBuff.mConstBufferData->mDirLights[i].active = true;
			mConstBuff.mConstBufferData->mDirLights[i].direction = -mDirLights[i].mDirection;
			mConstBuff.mConstBufferData->mDirLights[i].color = mDirLights[i].mColor;
			mConstBuff.mConstBufferData->mAmbienColor = mAmbienColor;
		}
		else
		{
			mConstBuff.mConstBufferData->mDirLights[i].active = false;
		}
	}
	for (int32_t i = 0; i < sPOINTLIGHT_NUM; i++)
	{
		if (mPointLights[i].mActive) {
			mConstBuff.mConstBufferData->mPointLights[i].active = true;
			mConstBuff.mConstBufferData->mPointLights[i].lightPos = mPointLights[i].mLightPos;
			mConstBuff.mConstBufferData->mPointLights[i].lightColor = mPointLights[i].mLightColor;
			mConstBuff.mConstBufferData->mPointLights[i].lighttAtten = mPointLights[i].mLightAtten;
		}
		else
		{
			mConstBuff.mConstBufferData->mPointLights[i].active = false;
		}
	}
}

void LightGroup::SetAmbientColor(const Vector3& color)
{
	mAmbienColor = color;
	mDirty = true;
}

void LightGroup::SetDirLightActive(const int32_t& index, const bool& active)
{
	assert(0 <= index && index < sDIRLIGHT_NUM);
	mDirLights[index].mActive = active;
}

void LightGroup::SetDirLightDir(const int32_t& index, const Vector3& lightdir)
{
	assert(0 <= index && index < sDIRLIGHT_NUM);
	mDirLights[index].mDirection = lightdir;
	mDirty = true;
}

void LightGroup::SetDirLightColor(const int32_t& index, const Vector3& lightcolor)
{
	assert(0 <= index && index < sDIRLIGHT_NUM);
	mDirLights[index].mColor = lightcolor;
	mDirty = true;
}

void LightGroup::SetPointLightActive(const int32_t& index, const bool& active)
{
	assert(0 <= index && index < sPOINTLIGHT_NUM);

	mPointLights[index].mActive = active;
}

void LightGroup::SetPointLightPos(const int32_t& index, const Vector3& pos)
{
	assert(0 <= index && index < sPOINTLIGHT_NUM);

	mPointLights[index].mLightPos = pos;
	mDirty = true;
}

void LightGroup::SetPointLightColor(const int32_t& index, const Vector3& color)
{
	assert(0 <= index && index < sPOINTLIGHT_NUM);

	mPointLights[index].mLightColor = color;
	mDirty = true;
}

void LightGroup::SetPointLightAtten(const int32_t& index, const Vector3& atten)
{
	assert(0 <= index && index < sPOINTLIGHT_NUM);

	mPointLights[index].mLightAtten = atten;
	mDirty = true;
}

void LightGroup::DefaultLightSet()
{
	mDirLights[0].mActive = true;
	mDirLights[0].mColor = { 1.0f,0.0f,0.0f };
	mDirLights[0].mDirection = { 0.0f,-1.0f,0.0f };
	
	mDirLights[1].mActive = true;
	mDirLights[1].mColor = { 0.0f,1.0f,0.0f };
	mDirLights[1].mDirection = { 0.5f,0.1f,0.2f };
	
	mDirLights[2].mActive = true;
	mDirLights[2].mColor = { 0.0f,0.0f,1.0f };
	mDirLights[2].mDirection = { -0.5f,0.1f,-0.2f };
}
