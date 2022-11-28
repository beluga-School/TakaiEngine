#include "Texture.h"
#include "Result.h"
#include "DirectXInit.h"
#include <memory>

void Texture::CreateWhiteTexture()
{
	const size_t textureWidth = 256;
	const size_t textureHeight = 256;

	const size_t imageDatacount = textureWidth * textureHeight;
	std::unique_ptr<XMFLOAT4[]> imageData;
	imageData = std::make_unique<XMFLOAT4[]>(imageDatacount);

	for (size_t i = 0; i < imageDatacount; i++)
	{
		imageData[i].x = 1.0f;
		imageData[i].y = 1.0f;
		imageData[i].z = 1.0f;
		imageData[i].w = 1.0f;
	}

	//���\�[�X�ݒ�
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty =
		D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureResourceDesc.Width = textureWidth;
	textureResourceDesc.Height = textureHeight;
	textureResourceDesc.DepthOrArraySize = 1;
	textureResourceDesc.MipLevels = 1;
	textureResourceDesc.SampleDesc.Count = 1;

	//�e�N�X�`���o�b�t�@�̐���
	result = dx12->device->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff));

	//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	result = texBuff->WriteToSubresource(
		0,
		nullptr,	//�S�̈�փR�s�[
		imageData.get(),	//���f�[�^�A�h���X
		sizeof(XMFLOAT4) * textureWidth,	//1���C���T�C�Y
		sizeof(XMFLOAT4) * imageDatacount	//�S�T�C�Y
	);
	assert(SUCCEEDED(result));

	TextureManager* tManager = TextureManager::GetInstance();

	//�n���h�����擾���镔��
	//gpu�n���h�����擾
	gpuHandle = tManager->srvHeap->GetGPUDescriptorHandleForHeapStart();

	//cpu�n���h�����擾
	cpuHandle = tManager->srvHeap->GetCPUDescriptorHandleForHeapStart();

	//�O���SRV�q�[�v�̑傫�������Z
	cpuHandle.ptr += tManager->SRVHandleSize;

	gpuHandle.ptr += tManager->SRVHandleSize;

	//SRV�q�[�v�̑傫�����擾
	tManager->SRVHandleSize += dx12->device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//�V�F�[�_���\�[�X�r���[�ݒ�
	srvDesc.Format = resDesc.Format;//RGBA float
	srvDesc.Shader4ComponentMapping =
		D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = resDesc.MipLevels;

	//�n���h���̎w���ʒu�ɃV�F�[�_�[���\�[�X�r���[�쐬
	dx12->device->CreateShaderResourceView(texBuff.Get(), &srvDesc, cpuHandle);

	getResDesc = textureResourceDesc;
}

void Texture::Load(const wchar_t* t)
{
	//���\�[�X�ݒ�
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	result = LoadFromWICFile(
		t,	//�����ɓǂݍ��݂����t�@�C���̃p�X������
		WIC_FLAGS_NONE,
		&metadata, scratchImg
	);

	//�~�b�v�}�b�v����
	result = GenerateMipMaps(
		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain);
	if (SUCCEEDED(result)) {
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	//�ǂݍ��񂾃f�B�t���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
	metadata.format = MakeSRGB(metadata.format);

	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty =
		D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = metadata.format;
	textureResourceDesc.Width = metadata.width;
	textureResourceDesc.Height = (UINT)metadata.height;
	textureResourceDesc.DepthOrArraySize = (UINT)metadata.arraySize;
	textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResourceDesc.SampleDesc.Count = 1;

	//�e�N�X�`���o�b�t�@�̐���
	result = dx12->device->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff));

	for (SIZE_T j = 0; j < metadata.mipLevels; j++)
	{
		//�~�b�v�}�b�v���x�����w�肵�ăC���[�W���擾
		const Image* img = scratchImg.GetImage(j, 0, 0);
		//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
		result = texBuff->WriteToSubresource(
			(UINT)j,
			nullptr,	//�S�̈�փR�s�[
			img->pixels,	//���f�[�^�A�h���X
			(UINT)img->rowPitch,	//1���C���T�C�Y
			(UINT)img->slicePitch	//�S�T�C�Y
		);
		assert(SUCCEEDED(result));
	}

	TextureManager *tManager = TextureManager::GetInstance();

	//�n���h�����擾���镔��
	//gpu�n���h�����擾
	gpuHandle = tManager->srvHeap->GetGPUDescriptorHandleForHeapStart();

	//cpu�n���h�����擾
	cpuHandle = tManager->srvHeap->GetCPUDescriptorHandleForHeapStart();

	//SRV�q�[�v�̑傫�������Z
	cpuHandle.ptr += tManager->SRVHandleSize;

	gpuHandle.ptr += tManager->SRVHandleSize;

	//SRV�q�[�v�̑傫�����擾
	tManager->SRVHandleSize += dx12->device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//�V�F�[�_���\�[�X�r���[�ݒ�
	srvDesc.Format = resDesc.Format;//RGBA float
	srvDesc.Shader4ComponentMapping =
		D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = resDesc.MipLevels;

	//�n���h���̎w���ʒu�ɃV�F�[�_�[���\�[�X�r���[�쐬
	dx12->device->CreateShaderResourceView(texBuff.Get(), &srvDesc, cpuHandle);

	getResDesc = textureResourceDesc;
}

void TextureManager::Initialize()
{
	DirectX12 *dx12 = DirectX12::GetInstance();

	//�f�X�N���v�^�q�[�v�̐ݒ�
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//�V�F�[�_�[���猩����悤��
	srvHeapDesc.NumDescriptors = kMaxSRVCount;

	//�ݒ������SRV�p�f�X�N���v�^�q�[�v�𐶐�
	result = dx12->device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));
	assert(SUCCEEDED(result));
}

void TextureManager::PreLoad()
{
	//white.CreateWhiteTexture();
	white.Load(L"Resources\\white.png");
	slime.Load(L"Resources\\slime.png");
	def.Load(L"Resources\\default.png");
	pizza.Load(L"Resources\\pizza.png");
	particle.Load(L"Resources\\particle.png");
}
