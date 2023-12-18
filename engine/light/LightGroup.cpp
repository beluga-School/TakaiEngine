#include "LightGroup.h"
#include "DirectXInit.h"
#include "Player.h"
#include "MathF.h"

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
			mConstBuff.mConstBufferData->mPointLights[i].lightColor = { mPointLights[i].mLightColor.x,mPointLights[i].mLightColor .y,mPointLights[i].mLightColor .z,1};
			mConstBuff.mConstBufferData->mPointLights[i].intensity = mPointLights[i].intensity;
			mConstBuff.mConstBufferData->mPointLights[i].decay = mPointLights[i].decay;
			mConstBuff.mConstBufferData->mPointLights[i].radius = mPointLights[i].radius;
		}
		else
		{
			mConstBuff.mConstBufferData->mPointLights[i].active = false;
		}
	}

	for (int32_t i = 0; i < sCIRCLESHADOW_NUM; i++)
	{
		if (mCircleShadows[i].mActive)
		{
			mConstBuff.mConstBufferData->mCircleShadows[i].active = true;
			mConstBuff.mConstBufferData->mCircleShadows[i].direction = -mCircleShadows[i].mDirection;
			mConstBuff.mConstBufferData->mCircleShadows[i].casterPos = mCircleShadows[i].casterPos;
			mConstBuff.mConstBufferData->mCircleShadows[i].distance = mCircleShadows[i].distance;
			mConstBuff.mConstBufferData->mCircleShadows[i].atten = mCircleShadows[i].atten;
			mConstBuff.mConstBufferData->mCircleShadows[i].factorAngleCos = mCircleShadows[i].factorAngleCos;
		}
		else
		{
			mConstBuff.mConstBufferData->mCircleShadows[i].active = false;
		}
	}
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
	mDirLights[0].mColor = { 1.0f,1.0f,1.0f };
	mDirLights[0].mDirection = { 0.0f,-0.8f,0.0f };

	mDirLights[1].mActive = false;
	mDirLights[1].mColor = { 0.0f,1.0f,0.0f };
	mDirLights[1].mDirection = { 0.5f,0.1f,0.2f };

	mDirLights[2].mActive = false;
	mDirLights[2].mColor = { 1.0f,1.0f,1.0f };
	mDirLights[2].mDirection = { -0.5f,0.1f,-0.2f };

	mPointLights[0].mActive = false;
	mPointLights[0].mLightColor = {1.0f,1.0f,1.0f};
	mPointLights[0].mLightAtten = { 1.0f ,1.0f,1.0f};

	mCircleShadows[0].mActive = true;
}

void LightGroup::LightDebugGUI()
{

	lightGui.Begin({800,100},{200,200});
	
	//PointLightDebug();
	SpotLightDebug();

	lightGui.End();

	//重い処理なんで避ける工夫を
	TransferBuffer();
}

void LightGroup::SpotLightDebug()
{
	static float angle = 0;
	static float fallStart = 0;


	ImGui::SliderFloat("mDirection.x", &mCircleShadows[0].mDirection.x,-1.f,1.0f);
	ImGui::SliderFloat("mDirection.y", &mCircleShadows[0].mDirection.y,-1.f,1.0f);
	ImGui::SliderFloat("mDirection.z", &mCircleShadows[0].mDirection.z,-1.f,1.0f);
	ImGui::SliderFloat("atten.x", &mCircleShadows[0].atten.x,-10.f,10.0f);
	ImGui::SliderFloat("atten.y", &mCircleShadows[0].atten.y,-10.f,10.0f);
	ImGui::SliderFloat("atten.z", &mCircleShadows[0].atten.z,-10.f,10.0f);
	ImGui::SliderFloat("factorAngleCos.x", &mCircleShadows[0].factorAngleCos.x,-MathF::PIf, MathF::PIf);
	ImGui::SliderFloat("factorAngleCos.y", &mCircleShadows[0].factorAngleCos.y,-MathF::PIf, MathF::PIf);
	ImGui::SliderFloat("distance", &mCircleShadows[0].distance,0.f,5.0f);

	mCircleShadows[0].casterPos = Player::Get()->position;
	//mCircleShadows[0].casterPos.y -= 10;

	if (ImGui::Button("LightPlayerSet"))
	{
		LightGroup::Get()->mCircleShadows[0].casterPos = Player::Get()->position;
	}
}

void LightGroup::PointLightDebug()
{
	ImGui::SliderFloat("pointLight:intensity", &mPointLights[0].intensity, 0, 1.0f);
	ImGui::SliderFloat("pointLight:pos.x", &mPointLights[0].mLightPos.x, -200.f, 200.0f);
	ImGui::SliderFloat("pointLight:pos.y", &mPointLights[0].mLightPos.y, -200.f, 200.0f);
	ImGui::SliderFloat("pointLight:pos.z", &mPointLights[0].mLightPos.z, -200.f, 200.0f);
	ImGui::SliderFloat("pointLight:distance", &mPointLights[0].radius, 0.0f, 100.0f);
	ImGui::SliderFloat("pointLight:decay", &mPointLights[0].decay, 0.0f, 100.0f);

	if (ImGui::Button("LightPlayerSet"))
	{
		LightGroup::Get()->mPointLights[0].mLightPos = Player::Get()->position;
	}
}
