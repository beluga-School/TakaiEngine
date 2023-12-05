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

	for (int32_t i = 0; i < sSPOTLIGHT_NUM; i++)
	{
		if (mSpotLights[i].mActive)
		{
			mConstBuff.mConstBufferData->mSpotLights[i].active = true;
			mConstBuff.mConstBufferData->mSpotLights[i].lightPos = mSpotLights[i].mLightPos;
			mConstBuff.mConstBufferData->mSpotLights[i].lightColor = { mSpotLights[i].mLightColor.x,mSpotLights[i].mLightColor.y,mSpotLights[i].mLightColor.z,1 };
			mConstBuff.mConstBufferData->mSpotLights[i].intensity = mSpotLights[i].intensity;
			mConstBuff.mConstBufferData->mSpotLights[i].decay = mSpotLights[i].decay;
			
			mConstBuff.mConstBufferData->mSpotLights[i].distance = mSpotLights[i].distance;
			mConstBuff.mConstBufferData->mSpotLights[i].cosAngle = mSpotLights[i].cosAngle;
			mConstBuff.mConstBufferData->mSpotLights[i].cosFalloffStart = mSpotLights[i].cosFalloffStart;
		}
		else
		{
			mConstBuff.mConstBufferData->mSpotLights[i].active = false;
		}
	}

	for (int32_t i = 0; i < sCIRCLESHADOW_NUM; i++)
	{
		if (mCircleShadow[i].mActive)
		{
			mConstBuff.mConstBufferData->mCircleShadow[i].active = true;
			mConstBuff.mConstBufferData->mCircleShadow[i].angleCos = mCircleShadow[i].factorAngleCos;
			mConstBuff.mConstBufferData->mCircleShadow[i].atten = mCircleShadow[i].atten;
			mConstBuff.mConstBufferData->mCircleShadow[i].casterPos = mCircleShadow[i].casterPos;
			mConstBuff.mConstBufferData->mCircleShadow[i].distance = mCircleShadow[i].distance;
			mConstBuff.mConstBufferData->mCircleShadow[i].direction = mCircleShadow[i].direction;
		
		}
		else
		{
			mConstBuff.mConstBufferData->mCircleShadow[i].active = false;
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

	mPointLights[0].mActive = true;
	mPointLights[0].mLightColor = {1.0f,1.0f,1.0f};
	mPointLights[0].mLightAtten = { 1.0f ,1.0f,1.0f};
	mPointLights[0].decay = 2.0f;
	mPointLights[0].intensity = 0.2f;
	mPointLights[0].radius = MathF::Avarage(Player::Get()->scale) * 2;

	mSpotLights[0].mActive = false;
	mSpotLights[0].mLightColor = { 1.0f,1.0f,1.0f };
	mSpotLights[0].mLightAtten = { 1.0f ,1.0f,1.0f };

	mCircleShadow[0].mActive = false;
}

void LightGroup::LightDebugGUI()
{

	lightGui.Begin({800,100},{200,200});
	
	PointLightDebug();
	CircleShadowDebug();
	
	lightGui.End();

	//重い処理なんで避ける工夫を
	TransferBuffer();
}

void LightGroup::SpotLightDebug()
{
	static float angle = 0;
	static float fallStart = 0;

	ImGui::SliderFloat("pointLight:intensity", &mSpotLights[0].intensity,0,1.0f);
	ImGui::SliderFloat("pointLight:pos.x", &mSpotLights[0].mLightPos.x,-200.f,200.0f);
	ImGui::SliderFloat("pointLight:pos.y", &mSpotLights[0].mLightPos.y,-200.f,200.0f);
	ImGui::SliderFloat("pointLight:pos.z", &mSpotLights[0].mLightPos.z,-200.f,200.0f);
	ImGui::SliderFloat("pointLight:distance", &mSpotLights[0].distance,0.0f,100.0f);
	ImGui::SliderFloat("pointLight:decay", &mSpotLights[0].decay,0.0f, 100.0f);
	ImGui::SliderFloat("pointLight:cosAngle", &angle,0.0f, 360.0f);
	ImGui::SliderFloat("pointLight:cosFalloffStart", &fallStart,1.0f, 360.0f);
	ImGui::SliderFloat("pointLight:mDirection.x", &mSpotLights[0].mDirection.x, -1.0f, 1.0f);
	ImGui::SliderFloat("pointLight:mDirection.y", &mSpotLights[0].mDirection.y, -1.0f, 1.0f);
	ImGui::SliderFloat("pointLight:mDirection.z", &mSpotLights[0].mDirection.z, -1.0f, 1.0f);
	
	mSpotLights[0].cosAngle = MathF::AngleConvRad(angle);
	mSpotLights[0].cosFalloffStart = MathF::AngleConvRad(fallStart);
	if (ImGui::Button("LightPlayerSet"))
	{
		LightGroup::Get()->mPointLights[0].mLightPos = Player::Get()->position;
	}
}

void LightGroup::CircleShadowDebug()
{
	ImGui::SliderFloat("circleShadow:distance", &mCircleShadow[0].distance, 0, 1.0f);
	ImGui::SliderFloat("circleShadow:atten.x", &mCircleShadow[0].atten.x, -200.f, 200.0f);
	ImGui::SliderFloat("circleShadow:atten.y", &mCircleShadow[0].atten.y, -200.f, 200.0f);
	ImGui::SliderFloat("circleShadow:atten.z", &mCircleShadow[0].atten.z, -200.f, 200.0f);
	ImGui::SliderFloat("circleShadow:factorAngleCos.x", &mCircleShadow[0].factorAngleCos.x, 0.0f, 100.0f);
	ImGui::SliderFloat("circleShadow:factorAngleCos.y", &mCircleShadow[0].factorAngleCos.y, 0.0f, 100.0f);

	if (ImGui::Button("LightPlayerSet"))
	{
		LightGroup::Get()->mCircleShadow[0].casterPos = Player::Get()->position;
	}
}

void LightGroup::PointLightDebug()
{
	ImGui::SliderFloat("pointLight:intensity", &mPointLights[0].intensity, 0, 1.0f);
	ImGui::SliderFloat("pointLight:distance", &mPointLights[0].radius, 0.0f, 100.0f);
	ImGui::SliderFloat("pointLight:decay", &mPointLights[0].decay, 0.0f, 100.0f);

	mPointLights[0].mLightPos = Player::Get()->position;

	if (ImGui::Button("LightPlayerSet"))
	{
		LightGroup::Get()->mPointLights[0].mLightPos = Player::Get()->position;
	}
}
