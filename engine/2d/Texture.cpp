#include "Texture.h"
#include "Result.h"
#include "DirectXInit.h"
#include <memory>

std::map<std::string, Texture> TextureManager::sTextures;

void Texture::CreateWhiteTexture()
{
	const size_t TEXTURE_WIDTH = 256;
	const size_t TEXTURE_HEIGHT = 256;

	const size_t IMAGE_DATA_COUNT = TEXTURE_WIDTH * TEXTURE_HEIGHT;
	std::unique_ptr<XMFLOAT4[]> imageData;
	imageData = std::make_unique<XMFLOAT4[]>(IMAGE_DATA_COUNT);

	for (size_t i = 0; i < IMAGE_DATA_COUNT; i++)
	{
		imageData[i].x = 1.0f;
		imageData[i].y = 1.0f;
		imageData[i].z = 1.0f;
		imageData[i].w = 1.0f;
	}

	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty =
		D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	//textureResourceDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureResourceDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	textureResourceDesc.Width = TEXTURE_WIDTH;
	textureResourceDesc.Height = TEXTURE_HEIGHT;
	textureResourceDesc.DepthOrArraySize = 1;
	textureResourceDesc.MipLevels = 1;
	textureResourceDesc.SampleDesc.Count = 1;

	//�e�N�X�`���o�b�t�@�̐���
	sResult = DirectX12::Get()->mDevice->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&mTexBuff));

	//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	sResult = mTexBuff->WriteToSubresource(
		0,
		nullptr,	//�S�̈�փR�s�[
		imageData.get(),	//���f�[�^�A�h���X
		sizeof(XMFLOAT4) * TEXTURE_WIDTH,	//1���C���T�C�Y
		sizeof(XMFLOAT4) * IMAGE_DATA_COUNT	//�S�T�C�Y
	);
	assert(SUCCEEDED(sResult));

	TextureManager* tManager = TextureManager::Get();

	//�n���h�����擾���镔��
	//gpu�n���h�����擾
	mGpuHandle = tManager->mSrvHeap->GetGPUDescriptorHandleForHeapStart();

	//cpu�n���h�����擾
	mCpuHandle = tManager->mSrvHeap->GetCPUDescriptorHandleForHeapStart();

	//�O���SRV�q�[�v�̑傫�������Z
	mCpuHandle.ptr += tManager->mSRVHandleSize;

	mGpuHandle.ptr += tManager->mSRVHandleSize;

	//SRV�q�[�v�̑傫�����擾
	tManager->mSRVHandleSize += DirectX12::Get()->mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//�V�F�[�_�[���\�[�X�ݒ�
	mResDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	mResDesc.DepthOrArraySize = 1;
	mResDesc.MipLevels = 1;
	mResDesc.SampleDesc.Count = 1;
	mResDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//�V�F�[�_���\�[�X�r���[�ݒ�
	mSrvDesc.Format = mResDesc.Format;//RGBA float
	mSrvDesc.Shader4ComponentMapping =
		D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	mSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	mSrvDesc.Texture2D.MipLevels = mResDesc.MipLevels;

	//�n���h���̎w���ʒu�ɃV�F�[�_�[���\�[�X�r���[�쐬
	DirectX12::Get()->mDevice->CreateShaderResourceView(mTexBuff.Get(), &mSrvDesc, mCpuHandle);

	mGetResDesc = textureResourceDesc;
}

void Texture::Load(const wchar_t& t)
{
	sResult = LoadFromWICFile(
		&t,	//�����ɓǂݍ��݂����t�@�C���̃p�X������
		WIC_FLAGS_NONE,
		&mMetadata, mScratchImg
	);

	//�~�b�v�}�b�v����
	sResult = GenerateMipMaps(
		mScratchImg.GetImages(), mScratchImg.GetImageCount(), mScratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mMipChain);
	if (SUCCEEDED(sResult)) {
		mScratchImg = std::move(mMipChain);
		mMetadata = mScratchImg.GetMetadata();
	}

	//�ǂݍ��񂾃f�B�t���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
	mMetadata.format = MakeSRGB(mMetadata.format);

	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty =
		D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = mMetadata.format;
	textureResourceDesc.Width = mMetadata.width;
	textureResourceDesc.Height = (UINT)mMetadata.height;
	textureResourceDesc.DepthOrArraySize = (UINT)mMetadata.arraySize;
	textureResourceDesc.MipLevels = (UINT16)mMetadata.mipLevels;
	textureResourceDesc.SampleDesc.Count = 1;

	//�e�N�X�`���o�b�t�@�̐���
	sResult = DirectX12::Get()->mDevice->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&mTexBuff));

	for (SIZE_T j = 0; j < mMetadata.mipLevels; j++)
	{
		//�~�b�v�}�b�v���x�����w�肵�ăC���[�W���擾
		const Image* img = mScratchImg.GetImage(j, 0, 0);
		//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
		sResult = mTexBuff->WriteToSubresource(
			(UINT)j,
			nullptr,	//�S�̈�փR�s�[
			img->pixels,	//���f�[�^�A�h���X
			(UINT)img->rowPitch,	//1���C���T�C�Y
			(UINT)img->slicePitch	//�S�T�C�Y
		);
		assert(SUCCEEDED(sResult));
	}

	TextureManager *tManager = TextureManager::Get();

	//�n���h�����擾���镔��
	//gpu�n���h�����擾
	mGpuHandle = tManager->mSrvHeap->GetGPUDescriptorHandleForHeapStart();

	//cpu�n���h�����擾
	mCpuHandle = tManager->mSrvHeap->GetCPUDescriptorHandleForHeapStart();

	//SRV�q�[�v�̑傫�������Z
	mCpuHandle.ptr += tManager->mSRVHandleSize;

	mGpuHandle.ptr += tManager->mSRVHandleSize;

	//SRV�q�[�v�̑傫�����擾
	tManager->mSRVHandleSize += DirectX12::Get()->mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//���\�[�X�ݒ�
	mResDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	mResDesc.DepthOrArraySize = 1;
	mResDesc.MipLevels = 1;
	mResDesc.SampleDesc.Count = 1;
	mResDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//�V�F�[�_���\�[�X�r���[�ݒ�
	mSrvDesc.Format = mResDesc.Format;//RGBA float
	mSrvDesc.Shader4ComponentMapping =
		D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	mSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	mSrvDesc.Texture2D.MipLevels = mResDesc.MipLevels;

	//�n���h���̎w���ʒu�ɃV�F�[�_�[���\�[�X�r���[�쐬
	DirectX12::Get()->mDevice->CreateShaderResourceView(mTexBuff.Get(), &mSrvDesc, mCpuHandle);

	mGetResDesc = textureResourceDesc;
}

void TextureManager::Initialize()
{
	DirectX12 *dx12 = DirectX12::Get();

	//�f�X�N���v�^�q�[�v�̐ݒ�
	mSrvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	mSrvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//�V�F�[�_�[���猩����悤��
	mSrvHeapDesc.NumDescriptors = K_MAX_SRV_COUNT;

	//�ݒ������SRV�p�f�X�N���v�^�q�[�v�𐶐�
	sResult = dx12->mDevice->CreateDescriptorHeap(&mSrvHeapDesc, IID_PPV_ARGS(&mSrvHeap));
	assert(SUCCEEDED(sResult));
}

void TextureManager::PreLoad()
{
	bugfix_.CreateWhiteTexture();

	Load("Resources\\white.png", "white");
	Load("Resources\\slime.png", "slime");
	Load("Resources\\default.png", "default");
	Load("Resources\\pizza.png", "pizza");
	Load("Resources\\particle.png", "particle");
	Load("Resources\\goal.png", "goal");
	Load("Resources\\blockTexture.png", "blockTexture");
	Load("Resources\\firewisp\\tex.png", "firewisp");
	Load("Resources\\Title.png", "Title");
	Load("Resources\\Start.png", "Start");
	Load("Resources\\Space.png", "Space");
	Load("Resources\\goalBlock.png", "goalBlock");
	Load("Resources\\dashIcon.png", "dashIcon");
	Load("Resources\\dashCool.png", "dashCool");
	Load("Resources\\hpbar.png", "hpbar");
	Load("Resources\\redScreen.png", "redScreen");
	Load("Resources\\GameOver.png", "GameOver");
}

//�l�b�g�ŏE���Ă���string->wstring�ϊ�
std::wstring convString(const std::string& input)
{
	size_t i;
	wchar_t* buffer = new wchar_t[input.size() + 1];
	mbstowcs_s(&i, buffer, input.size() + 1, input.c_str(), _TRUNCATE);
	std::wstring result = buffer;
	delete[] buffer;
	return result;
}

void TextureManager::Load(const std::string &filepath, const std::string &handle)
{
	sTextures[handle].Load(*convString(filepath).c_str());
}

Texture* TextureManager::GetTexture(const std::string &handle)
{
	return &sTextures[handle];
}
