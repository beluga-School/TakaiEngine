#include "Particle.h"
#include "DirectXInit.h"
#include "TimeManager.h"
#include "MathF.h"
#include "ViewProjection.h"

void ParticleManager::CreateParticle(XMFLOAT3 spawnPos, XMFLOAT3 velocity, float scale, float speed, bool redChange, float maxLifeTime, XMFLOAT4 color)
{
	particles.emplace_back(spawnPos, velocity, scale, speed,maxLifeTime,color,redChange);
}

void ParticleManager::Initialize()
{
	DirectX12 *dx12 = DirectX12::GetInstance();

	//頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(vertexCount * sizeof(VertexPos));

	//頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapProp{};		//ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPUへの転送用

	//リソース設定
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	//頂点データ全体のサイズ
	resDesc.Width = sizeVB;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//頂点バッファの生成
	//ComPtrにしたらダメだった マップ処理に使ってるから？
	//ID3D12Resource* vertBuff;
	result = dx12->device->CreateCommittedResource(
		&heapProp,	//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc,	//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	//GPU仮想アドレス
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	//頂点バッファのサイズ
	vbView.SizeInBytes = sizeVB;
	//頂点1つ分のデータサイズ
	vbView.StrideInBytes = sizeof(VertexPos);
}

void ParticleManager::Update(XMMATRIX& matView, XMMATRIX& matProjection)
{
	particles.remove_if([](Particle& particle)
		{
			return particle.isDead;
		});

	for (Particle& particle : particles) {
	
		particle.Update();
	}

	XMMATRIX reverseMatView;
	reverseMatView = XMMatrixInverse(nullptr, matView);
	reverseMatView.r[3] = XMVectorSet(0, 0, 0, 1);

	constBufferParticle.constBufferData->billboardMat = reverseMatView;

	constBufferParticle.constBufferData->vpMat = matView * matProjection;

	VertexPos* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	//全頂点に対して
	size_t index = 0;
	for (Particle& particle : particles) {
		vertMap[index].pos = particle.position;	//	座標をコピー
		vertMap[index].scale = particle.scale;	//	スケールをコピー
		vertMap[index].color = particle.color;	//	カラーをコピー
		index++;
		if (index > vertexCount)break;
	}
	//繋がりを解除
	vertBuff->Unmap(0, nullptr);

}

void ParticleManager::Draw(Texture* texture)
{
	DirectX12* dx12 = DirectX12::GetInstance();
	TextureManager* texM = TextureManager::GetInstance();

	//SRVヒープの先頭から順番にSRVをルートパラメータ1番に設定
	//ルートパラメータ1番はテクスチャバッファ
	dx12->commandList->SetGraphicsRootDescriptorTable(1, texture->gpuHandle);

	//頂点バッファの設定
	dx12->commandList->IASetVertexBuffers(0, 1, &vbView);

	//定数バッファビュー(CBV)の設定コマンド
	//dx12->commandList->SetGraphicsRootConstantBufferView(0, constBufferMaterial.buffer->GetGPUVirtualAddress());
	//commandList->SetGraphicsRootConstantBufferView(0, constBufferM.buffer->GetGPUVirtualAddress());

	dx12->commandList->SetGraphicsRootConstantBufferView(0, constBufferParticle.buffer->GetGPUVirtualAddress());

	//描画コマンド
	dx12->commandList->DrawInstanced(min(particles.size(),vertexCount), 1, 0, 0);
}

ParticleManager *ParticleManager::Getinstance()
{
	static ParticleManager instance;
	return &instance;
}

Particle::Particle(XMFLOAT3 spawnPos, XMFLOAT3 velocity, float scale, float speed, float maxLifeTime, XMFLOAT4 color, bool redChange)
{
	this->position = spawnPos;
	this->velocity = velocity;
	this->speed = speed;

	this->maxLifeTime = maxLifeTime;

	this->scale = scale;

	this->color = color;

	colorRand.x = MathF::GetRand(0.1f, 1);
	colorRand.y = MathF::GetRand(0.1f, 1);
	colorRand.z = MathF::GetRand(0.1f, 1);

	this->redChange = redChange;
}

void Particle::Update()
{
	lifeTime += TimeManager::deltaTime;
	if (lifeTime >= maxLifeTime)
	{
		isDead = true;
	}
	if (scale < 0.1f)
	{
		isDead = true;
	}

	position.x += (velocity.x * speed) * TimeManager::deltaTime;
	position.y += (velocity.y * speed) * TimeManager::deltaTime;
	position.z += (velocity.z * speed) * TimeManager::deltaTime;

	scale -= 1.0f * TimeManager::deltaTime;
	
	if (redChange)
	{
		color = { 1 - lifeTime, lifeTime ,0.0f,1.0f };
	}


	vertPos.pos = position;
}

void ParticleEmitter::Initialize()
{
	emitter.Initialize();
	emitter.position = { 0,0,0 };
	emitter.SetModel(&ModelManager::GetInstance()->cubeM);
	emitter.SetTexture(&TextureManager::GetInstance()->white);
}

void ParticleEmitter::SetInfo(Vector3 pos, float range, float scale, XMFLOAT4 color, int spawnNum, bool redChange)
{
	emitter.position = {
		pos.x,
		pos.y,
		pos.z
	};

	this->range = range;
	this->initScale = scale;
	if (spawnNum <= 0)
	{
		spawnNum = 1;
	}
	this->color = color;
	this->spawnNum = spawnNum;

	this->redChange = redChange;

}

void ParticleEmitter::Update()
{
	ParticleManager* pManager = ParticleManager::Getinstance();

	for (int i = 0; i < spawnNum; i++)
	{
		pManager->CreateParticle({
	emitter.position.x + MathF::GetRand(-range,range),
	emitter.position.y + MathF::GetRand(-range,range),
	emitter.position.z + MathF::GetRand(-range,range) },
				{
					MathF::GetRand(-10,10),
					MathF::GetRand(-10,10),
					MathF::GetRand(-10,10),
				},
				initScale,
				1,
				redChange,
				1,
				color);
	}

	emitter.Update(Camera::camera->matView, Camera::camera->matProjection);
}