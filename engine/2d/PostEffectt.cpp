#include "PostEffectt.h"
#include "util.h"
#include "Result.h"
#include <memory>

PostEffect::PostEffect() 
	: Sprite(
	)
{
	mTEXTURE = TextureManager::GetTexture("white");
	mPosition = { 0,0,0};
	mSize = { 500.f,500.f };
	mColor = { 1,1,1,1 };
	mAnchorpoint = { 0.f,0.f };

	Init();
}

void PostEffect::Initialize()
{
	DirectX12* dx12 = DirectX12::Get();

	//�v���O�����ō쐬�����e�N�X�`����\����K
	//texturemanager��white�e�N�X�`���𐶐������菇�Ɠ����悤�Ȃ��Ƃ�����΂���
	//�������Ă����܂������Ȃ������̂ŁA���̐l�̂���݂�

	//mTEXTURE�̕�����u����������΂���

	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	textureResourceDesc.Width = Util::WIN_WIDTH;
	textureResourceDesc.Height = (UINT)Util::WIN_HEIGHT;
	textureResourceDesc.DepthOrArraySize = 1;
	textureResourceDesc.MipLevels = 1;
	textureResourceDesc.SampleDesc.Count = 1;
	textureResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty =
		D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	//�e�N�X�`���o�b�t�@�̐���
	sResult = dx12->mDevice->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		nullptr,
		IID_PPV_ARGS(&mTexBuff));
	assert(SUCCEEDED(sResult));

	{//�e�N�X�`����ԂŃN���A
		
		//�c���̃s�N�Z�����̉�f��
		const UINT pixelCount = Util::WIN_WIDTH * Util::WIN_HEIGHT;

		const UINT rowPitch = sizeof(UINT) * Util::WIN_WIDTH;

		const UINT depthPitch = rowPitch * Util::WIN_HEIGHT;

		UINT* img = new UINT[pixelCount];
		for (int32_t i = 0; i < pixelCount; i++)
		{
			img[i] = 0xff0000ff;
		}

		//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
		sResult = mTexBuff->WriteToSubresource(
			0, nullptr,
			img, rowPitch, depthPitch
		);
		assert(SUCCEEDED(sResult));
		delete[] img;
	}

	///--�V����SRV�p�f�X�N���v�^�q�[�v���쐬
	//SRV�p�f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeaoDesc = {};
	
	//�f�X�N���v�^�q�[�v�̐ݒ�
	srvDescHeaoDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeaoDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//�V�F�[�_�[���猩����悤��
	srvDescHeaoDesc.NumDescriptors = 1; //�����_�[�e�N�X�`����p�̃q�[�v�Ȃ��߁A�T�C�Y��1�ł���

	//�ݒ������SRV�p�f�X�N���v�^�q�[�v�𐶐�
	sResult = dx12->mDevice->CreateDescriptorHeap(&srvDescHeaoDesc, IID_PPV_ARGS(&mDescHeapSRV));
	assert(SUCCEEDED(sResult));

	//�V�F�[�_���\�[�X�r���[�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC mSrvDesc{};
	mSrvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	mSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	mSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	mSrvDesc.Texture2D.MipLevels = 1;

	//�n���h���̎w���ʒu�ɃV�F�[�_�[���\�[�X�r���[�쐬
	dx12->mDevice->CreateShaderResourceView(mTexBuff.Get(), &mSrvDesc, mDescHeapSRV->GetCPUDescriptorHandleForHeapStart());
}

void PostEffect::Draw()
{
	DirectX12* dx12 = DirectX12::Get();
	TextureManager* texM = TextureManager::Get();

	if (mIsInvisible)
	{
		return;
	}

	Sprite::Update();

	//�p�C�v���C���X�e�[�g�̐ݒ�
	dx12->mCmdList->SetPipelineState(SpriteCommon::sSpriteCommon.mPipelineSet.mPipelinestate.Get());
	//���[�g�V�O�l�`���̐ݒ�
	dx12->mCmdList->SetGraphicsRootSignature(SpriteCommon::sSpriteCommon.mPipelineSet.mRootsignature.Get());
	//�v���~�e�B�u�`���ݒ�
	dx12->mCmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//GetAddressOf���Ȃ��ƃo�O��܂� ���R�͖Y�ꂿ����� ComPtr�����炾�����H
	dx12->mCmdList->SetDescriptorHeaps(1, mDescHeapSRV.GetAddressOf());

	dx12->mCmdList->IASetVertexBuffers(0, 1, &mVbView);

	dx12->mCmdList->SetGraphicsRootDescriptorTable(1, mDescHeapSRV->GetGPUDescriptorHandleForHeapStart());

	dx12->mCmdList->SetGraphicsRootConstantBufferView(0, mConstBuffer.mBuffer->GetGPUVirtualAddress());

	dx12->mCmdList->DrawInstanced(4, 1, 0, 0);
}
