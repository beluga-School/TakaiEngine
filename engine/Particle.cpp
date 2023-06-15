#include "Particle.h"
#include "DirectXInit.h"
#include "TimeManager.h"
#include "MathF.h"
#include "ViewProjection.h"

void GParticleManager::CreateParticle(const Vector3& spawnPos, const Vector3& velocity,
	const float& scale, const float& speed, const bool& redChange,
	const float& maxLifeTime, const Vector4& color)
{
	mParticles.emplace_back(spawnPos, velocity, scale, speed,maxLifeTime,color,redChange);
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
	mResDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	//���_�f�[�^�S�̂̃T�C�Y
	mResDesc.Width = sizeVB;
	mResDesc.Height = 1;
	mResDesc.DepthOrArraySize = 1;
	mResDesc.MipLevels = 1;
	mResDesc.SampleDesc.Count = 1;
	mResDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//���_�o�b�t�@�̐���
	//ComPtr�ɂ�����_�������� �}�b�v�����Ɏg���Ă邩��H
	//ID3D12Resource* vertBuff;
	result = dx12->mDevice->CreateCommittedResource(
		&heapProp,	//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&mResDesc,	//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&mVertBuff));
	assert(SUCCEEDED(result));

	//GPU���z�A�h���X
	mVbView.BufferLocation = mVertBuff->GetGPUVirtualAddress();
	//���_�o�b�t�@�̃T�C�Y
	mVbView.SizeInBytes = sizeVB;
	//���_1���̃f�[�^�T�C�Y
	mVbView.StrideInBytes = sizeof(VertexPos);
}

void GParticleManager::Update(XMMATRIX& matView, XMMATRIX& matProjection)
{
	mParticles.remove_if([](Particle& particle)
		{
			return particle.mIsDead;
		});

	for (Particle& particle : mParticles) {
	
		particle.Update();
	}

	XMMATRIX reverseMatView;
	reverseMatView = XMMatrixInverse(nullptr, matView);
	reverseMatView.r[3] = XMVectorSet(0, 0, 0, 1);

	mConstBufferParticle.mConstBufferData->billboardMat = reverseMatView;

	mConstBufferParticle.mConstBufferData->vpMat = matView * matProjection;

	VertexPos* vertMap = nullptr;
	result = mVertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	//�S���_�ɑ΂���
	size_t index = 0;
	for (Particle& particle : mParticles) {
		vertMap[index].pos = particle.mPosition;	//	���W���R�s�[
		vertMap[index].scale = particle.mScale;	//	�X�P�[�����R�s�[
		vertMap[index].color = particle.mColor;	//	�J���[���R�s�[
		index++;
		if (index > vertexCount)break;
	}
	//�q���������
	mVertBuff->Unmap(0, nullptr);

}

void GParticleManager::Draw(Texture* texture)
{
	DirectX12* dx12 = DirectX12::Get();
	TextureManager* texM = TextureManager::Get();

	//SRV�q�[�v�̐擪���珇�Ԃ�SRV�����[�g�p�����[�^1�Ԃɐݒ�
	//���[�g�p�����[�^1�Ԃ̓e�N�X�`���o�b�t�@
	dx12->mCmdList->SetGraphicsRootDescriptorTable(1, texture->mGpuHandle);

	//���_�o�b�t�@�̐ݒ�
	dx12->mCmdList->IASetVertexBuffers(0, 1, &mVbView);

	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	//dx12->commandList->SetGraphicsRootConstantBufferView(0, constBufferMaterial.buffer->GetGPUVirtualAddress());
	//commandList->SetGraphicsRootConstantBufferView(0, constBufferM.buffer->GetGPUVirtualAddress());

	dx12->mCmdList->SetGraphicsRootConstantBufferView(0, mConstBufferParticle.mBuffer->GetGPUVirtualAddress());

	//�`��R�}���h
	dx12->mCmdList->DrawInstanced(min((UINT)mParticles.size(),vertexCount), 1, 0, 0);
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
	mPosition = spawnPos_;
	mVelocity = velocity_;
	mSpeed = speed_;

	mMaxLifeTime = maxLifeTime_;

	mScale = scale_;

	mColor = color_;

	mColorRand.x = MathF::GetRand(0.1f, 1);
	mColorRand.y = MathF::GetRand(0.1f, 1);
	mColorRand.z = MathF::GetRand(0.1f, 1);

	mRedChange = redChange_;

	mVertPos = {};
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
	
	if (mRedChange)
	{
		mColor = { 1 - mLifeTime, mLifeTime ,0.0f,1.0f };
	}


	mVertPos.pos = mPosition;
}

void ParticleEmitter::Initialize()
{
	mEmitter.Initialize();
	mEmitter.position = { 0,0,0 };
	mEmitter.SetModel(ModelManager::Get()->GetModel("Cube"));
	mEmitter.SetTexture(TextureManager::Get()->GetTexture("white"));
}

void ParticleEmitter::SetInfo(const Vector3& pos, const float& range_, const float& scale_,
	const Vector4& color_, const int32_t& spawnNum_, const bool& redChange_)
{
	mEmitter.position = {
		pos.x,
		pos.y,
		pos.z
	};

	mRange = range_;
	mInitScale = scale_;

	mColor = color_;
	mSpawnNum = spawnNum_;

	if (mSpawnNum <= 0)
	{
		mSpawnNum = 1;
	}

	mRedChange = redChange_;

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

	mEmitter.Update(*Camera::mCamera);
}