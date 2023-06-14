#include "Particle.h"
#include "DirectXInit.h"
#include "TimeManager.h"
#include "MathF.h"
#include "ViewProjection.h"

void GParticleManager::CreateParticle(const Vector3& spawnPos, const Vector3& velocity,
	const float& scale, const float& speed, const bool& redChange,
	const float& maxLifeTime, const Vector4& color)
{
	particles.emplace_back(spawnPos, velocity, scale, speed,maxLifeTime,color,redChange);
}

void GParticleManager::Initialize()
{
	DirectX12 *dx12 = DirectX12::Get();

	//���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB = static_cast<UINT>(vertexCount * sizeof(VertexPos));

	//���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProp{};		//�q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPU�ւ̓]���p

	//���\�[�X�ݒ�
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	//���_�f�[�^�S�̂̃T�C�Y
	resDesc.Width = sizeVB;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//���_�o�b�t�@�̐���
	//ComPtr�ɂ�����_�������� �}�b�v�����Ɏg���Ă邩��H
	//ID3D12Resource* vertBuff;
	result = dx12->device->CreateCommittedResource(
		&heapProp,	//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc,	//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	//GPU���z�A�h���X
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	//���_�o�b�t�@�̃T�C�Y
	vbView.SizeInBytes = sizeVB;
	//���_1���̃f�[�^�T�C�Y
	vbView.StrideInBytes = sizeof(VertexPos);
}

void GParticleManager::Update(XMMATRIX& matView, XMMATRIX& matProjection)
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
	//�S���_�ɑ΂���
	size_t index = 0;
	for (Particle& particle : particles) {
		vertMap[index].pos = particle.position;	//	���W���R�s�[
		vertMap[index].scale = particle.scale;	//	�X�P�[�����R�s�[
		vertMap[index].color = particle.color;	//	�J���[���R�s�[
		index++;
		if (index > vertexCount)break;
	}
	//�q���������
	vertBuff->Unmap(0, nullptr);

}

void GParticleManager::Draw(Texture* texture)
{
	DirectX12* dx12 = DirectX12::Get();
	TextureManager* texM = TextureManager::Get();

	//SRV�q�[�v�̐擪���珇�Ԃ�SRV�����[�g�p�����[�^1�Ԃɐݒ�
	//���[�g�p�����[�^1�Ԃ̓e�N�X�`���o�b�t�@
	dx12->commandList->SetGraphicsRootDescriptorTable(1, texture->gpuHandle);

	//���_�o�b�t�@�̐ݒ�
	dx12->commandList->IASetVertexBuffers(0, 1, &vbView);

	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	//dx12->commandList->SetGraphicsRootConstantBufferView(0, constBufferMaterial.buffer->GetGPUVirtualAddress());
	//commandList->SetGraphicsRootConstantBufferView(0, constBufferM.buffer->GetGPUVirtualAddress());

	dx12->commandList->SetGraphicsRootConstantBufferView(0, constBufferParticle.buffer->GetGPUVirtualAddress());

	//�`��R�}���h
	dx12->commandList->DrawInstanced(min((UINT)particles.size(),vertexCount), 1, 0, 0);
}

GParticleManager* GParticleManager::Getinstance()
{
	static GParticleManager instance;
	return &instance;
}

Particle::Particle(const Vector3& spawnPos_, const Vector3& velocity_,
	const float& scale_, const float& speed_, const float& maxLifeTime_,
	const Vector4& color_, const bool& redChange_)
{
	position = spawnPos_;
	velocity = velocity_;
	speed = speed_;

	maxLifeTime = maxLifeTime_;

	scale = scale_;

	color = color_;

	colorRand.x = MathF::GetRand(0.1f, 1);
	colorRand.y = MathF::GetRand(0.1f, 1);
	colorRand.z = MathF::GetRand(0.1f, 1);

	redChange = redChange_;

	vertPos = {};
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
	emitter.SetModel(ModelManager::Get()->GetModel("Cube"));
	emitter.SetTexture(&TextureManager::Get()->white);
}

void ParticleEmitter::SetInfo(const Vector3& pos, const float& range_, const float& scale_,
	const Vector4& color_, const int32_t& spawnNum_, const bool& redChange_)
{
	emitter.position = {
		pos.x,
		pos.y,
		pos.z
	};

	range = range_;
	initScale = scale_;

	color = color_;
	spawnNum = spawnNum_;

	if (spawnNum <= 0)
	{
		spawnNum = 1;
	}

	redChange = redChange_;

}

void ParticleEmitter::Update()
{
	GParticleManager* pManager = GParticleManager::Getinstance();

	for (int32_t i = 0; i < spawnNum; i++)
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

	emitter.Update(*Camera::camera);
}