#include "Particle.h"
#include "DirectXInit.h"
#include "TimeManager.h"
#include "MathF.h"
#include "ViewProjection.h"

void GParticleManager::CreateParticle(const Vector3& spawnPos, const Vector3& velocity,
	const float& scale, const float& speed, const bool& redChange,
	const float& maxLifeTime, const Float4& color)
{
	//mParticles.emplace_back(spawnPos, velocity, scale, speed,maxLifeTime,color,redChange);
}

void GParticleManager::Initialize()
{
	DirectX12 *dx12 = DirectX12::Get();

	//頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(VERTEX_COUNT * sizeof(VertexPos));

	//頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapProp{};		//ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPUへの転送用

	//リソース設定
	mResDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	//頂点データ全体のサイズ
	mResDesc.Width = sizeVB;
	mResDesc.Height = 1;
	mResDesc.DepthOrArraySize = 1;
	mResDesc.MipLevels = 1;
	mResDesc.SampleDesc.Count = 1;
	mResDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//頂点バッファの生成
	//ComPtrにしたらダメだった マップ処理に使ってるから？
	//ID3D12Resource* vertBuff;
	sResult = dx12->mDevice->CreateCommittedResource(
		&heapProp,	//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&mResDesc,	//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&mVertBuff));
	assert(SUCCEEDED(sResult));

	//GPU仮想アドレス
	mVbView.BufferLocation = mVertBuff->GetGPUVirtualAddress();
	//頂点バッファのサイズ
	mVbView.SizeInBytes = sizeVB;
	//頂点1つ分のデータサイズ
	mVbView.StrideInBytes = sizeof(VertexPos);
}

void GParticleManager::Update(DirectX::XMMATRIX& matView, DirectX::XMMATRIX& matProjection)
{
	mParticles.remove_if([](Particle& particle)
		{
			return particle.mIsDead;
		});

	for (Particle& particle : mParticles) {
	
		particle.Update();
	}

	DirectX::XMMATRIX reverseMatView;
	reverseMatView = XMMatrixInverse(nullptr, matView);
	reverseMatView.r[3] = DirectX::XMVectorSet(0, 0, 0, 1);

	mConstBufferParticle.mConstBufferData->billboardMat = reverseMatView;

	mConstBufferParticle.mConstBufferData->vpMat = matView * matProjection;

	VertexPos* vertMap = nullptr;
	sResult = mVertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(sResult));
	//全頂点に対して
	size_t index = 0;
	for (Particle& particle : mParticles) {
		vertMap[index].pos = particle.mPosition;	//	座標をコピー
		vertMap[index].scale = particle.mScale;	//	スケールをコピー
		vertMap[index].color = particle.mColor;	//	カラーをコピー
		index++;
		if (index > VERTEX_COUNT)break;
	}
	//繋がりを解除
	mVertBuff->Unmap(0, nullptr);

}

void GParticleManager::Draw(Texture* texture)
{
	DirectX12* dx12 = DirectX12::Get();
	TextureManager* texM = TextureManager::Get();

	//SRVヒープの先頭から順番にSRVをルートパラメータ1番に設定
	//ルートパラメータ1番はテクスチャバッファ
	dx12->mCmdList->SetGraphicsRootDescriptorTable(1, texture->mGpuHandle);

	//頂点バッファの設定
	dx12->mCmdList->IASetVertexBuffers(0, 1, &mVbView);

	//定数バッファビュー(CBV)の設定コマンド
	//dx12->commandList->SetGraphicsRootConstantBufferView(0, constBufferMaterial.buffer->GetGPUVirtualAddress());
	//commandList->SetGraphicsRootConstantBufferView(0, constBufferM.buffer->GetGPUVirtualAddress());

	dx12->mCmdList->SetGraphicsRootConstantBufferView(0, mConstBufferParticle.mBuffer->GetGPUVirtualAddress());

	//描画コマンド
	dx12->mCmdList->DrawInstanced(min((UINT)mParticles.size(),VERTEX_COUNT), 1, 0, 0);
}

GParticleManager* GParticleManager::Getinstance()
{
	static GParticleManager instance;
	return &instance;
}

void Particle::Update()
{
	mLifeTime += TimeManager::deltaTime;
	if (mLifeTime >= mMaxLifeTime)
	{
		mIsDead = true;
	}
	if (mScale < 0.1f)
	{
		mIsDead = true;
	}

	mPosition.x += (mVelocity.x * mSpeed) * TimeManager::deltaTime;
	mPosition.y += (mVelocity.y * mSpeed) * TimeManager::deltaTime;
	mPosition.z += (mVelocity.z * mSpeed) * TimeManager::deltaTime;

	mScale -= 1.0f * TimeManager::deltaTime;

}

void ParticleEmitter::Initialize()
{
	mEmitter.Initialize();
	mEmitter.position = { 0,0,0 };
	mEmitter.SetModel(ModelManager::Get()->GetModel("Cube"));
	mEmitter.SetTexture(TextureManager::Get()->GetTexture("white"));
}

void ParticleEmitter::Update()
{
	GParticleManager* pManager = GParticleManager::Getinstance();

	for (int32_t i = 0; i < mSpawnNum; i++)
	{
		pManager->CreateParticle({
	mEmitter.position.x + MathF::GetRand(-mRange,mRange),
	mEmitter.position.y + MathF::GetRand(-mRange,mRange),
	mEmitter.position.z + MathF::GetRand(-mRange,mRange) },
				{
					MathF::GetRand(-10,10),
					MathF::GetRand(-10,10),
					MathF::GetRand(-10,10),
				},
				mInitScale,
				1,
				mRedChange,
				1,
				mColor);
	}

	mEmitter.Update(*Camera::sCamera);
}