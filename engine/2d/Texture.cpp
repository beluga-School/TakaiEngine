#include "Texture.h"
#include "Result.h"
#include "DirectXInit.h"
#include <memory>
#include "Util.h"
#include <vector>
#include "StringUtil.h"

using namespace DirectX;

std::map<std::string, Texture> TextureManager::sTextures;

DirectX::TexMetadata* Texture::GetMetaData()
{
	return &mMetadata;
}

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

	//ヒープ設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty =
		D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	//リソース設定
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	//textureResourceDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureResourceDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	textureResourceDesc.Width = TEXTURE_WIDTH;
	textureResourceDesc.Height = TEXTURE_HEIGHT;
	textureResourceDesc.DepthOrArraySize = 1;
	textureResourceDesc.MipLevels = 1;
	textureResourceDesc.SampleDesc.Count = 1;

	//テクスチャバッファの生成
	sResult = DirectX12::Get()->mDevice->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&mTexBuff));

	//テクスチャバッファにデータ転送
	sResult = mTexBuff->WriteToSubresource(
		0,
		nullptr,	//全領域へコピー
		imageData.get(),	//元データアドレス
		sizeof(XMFLOAT4) * TEXTURE_WIDTH,	//1ラインサイズ
		sizeof(XMFLOAT4) * IMAGE_DATA_COUNT	//全サイズ
	);
	assert(SUCCEEDED(sResult));

	TextureManager* tManager = TextureManager::Get();

	//ハンドルを取得する部分
	//gpuハンドルを取得
	mGpuHandle = tManager->mSrvHeap->GetGPUDescriptorHandleForHeapStart();

	//cpuハンドルを取得
	mCpuHandle = tManager->mSrvHeap->GetCPUDescriptorHandleForHeapStart();

	//前回のSRVヒープの大きさを加算
	mCpuHandle.ptr += tManager->mSRVHandleSize;

	mGpuHandle.ptr += tManager->mSRVHandleSize;

	//SRVヒープの大きさを取得
	tManager->mSRVHandleSize += DirectX12::Get()->mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//シェーダーリソース設定
	mResDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	mResDesc.DepthOrArraySize = 1;
	mResDesc.MipLevels = 1;
	mResDesc.SampleDesc.Count = 1;
	mResDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//シェーダリソースビュー設定
	mSrvDesc.Format = mResDesc.Format;//RGBA float
	mSrvDesc.Shader4ComponentMapping =
		D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	mSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	mSrvDesc.Texture2D.MipLevels = mResDesc.MipLevels;

	//ハンドルの指す位置にシェーダーリソースビュー作成
	DirectX12::Get()->mDevice->CreateShaderResourceView(mTexBuff.Get(), &mSrvDesc, mCpuHandle);

	mGetResDesc = textureResourceDesc;
}

void Texture::Load(const wchar_t& t)
{
	std::string hoge = ConvertWCharToString(&t);

	std::vector<std::string> fileExt_ = Util::SplitString(hoge,".");
	if (fileExt_.back() == "dds") {
		sResult = LoadFromDDSFile(
			&t,	//ここに読み込みたいファイルのパスを入れる
			DDS_FLAGS_NONE,
			&mMetadata, mScratchImg
		);
	}
	else
	{
		sResult = LoadFromWICFile(
			&t,	//ここに読み込みたいファイルのパスを入れる
			WIC_FLAGS_NONE,
			&mMetadata, mScratchImg
		);
	}

	//ミップマップ生成
	sResult = GenerateMipMaps(
		mScratchImg.GetImages(), mScratchImg.GetImageCount(), mScratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mMipChain);
	if (SUCCEEDED(sResult)) {
		mScratchImg = std::move(mMipChain);
		mMetadata = mScratchImg.GetMetadata();
	}

	//読み込んだディフューズテクスチャをSRGBとして扱う
	mMetadata.format = MakeSRGB(mMetadata.format);

	//ヒープ設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty =
		D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	//リソース設定
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = mMetadata.format;
	textureResourceDesc.Width = mMetadata.width;
	textureResourceDesc.Height = (UINT)mMetadata.height;
	textureResourceDesc.DepthOrArraySize = (UINT16)mMetadata.arraySize;
	textureResourceDesc.MipLevels = (UINT16)mMetadata.mipLevels;
	textureResourceDesc.SampleDesc.Count = 1;

	//テクスチャバッファの生成
	sResult = DirectX12::Get()->mDevice->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&mTexBuff));

	for (SIZE_T j = 0; j < mMetadata.mipLevels; j++)
	{
		//ミップマップレベルを指定してイメージを取得
		const Image* img = mScratchImg.GetImage(j, 0, 0);
		//テクスチャバッファにデータ転送
		sResult = mTexBuff->WriteToSubresource(
			(UINT)j,
			nullptr,	//全領域へコピー
			img->pixels,	//元データアドレス
			(UINT)img->rowPitch,	//1ラインサイズ
			(UINT)img->slicePitch	//全サイズ
		);
		assert(SUCCEEDED(sResult));
	}

	TextureManager *tManager = TextureManager::Get();

	//ハンドルを取得する部分
	//gpuハンドルを取得
	mGpuHandle = tManager->mSrvHeap->GetGPUDescriptorHandleForHeapStart();

	//cpuハンドルを取得
	mCpuHandle = tManager->mSrvHeap->GetCPUDescriptorHandleForHeapStart();

	//SRVヒープの大きさを加算
	mCpuHandle.ptr += tManager->mSRVHandleSize;

	mGpuHandle.ptr += tManager->mSRVHandleSize;

	//SRVヒープの大きさを取得
	tManager->mSRVHandleSize += DirectX12::Get()->mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//リソース設定
	mResDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	mResDesc.DepthOrArraySize = 1;
	mResDesc.MipLevels = 1;
	mResDesc.SampleDesc.Count = 1;
	mResDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//シェーダリソースビュー設定
	mSrvDesc.Format = mResDesc.Format;//RGBA float
	mSrvDesc.Shader4ComponentMapping =
		D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	mSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	mSrvDesc.Texture2D.MipLevels = mResDesc.MipLevels;

	//ハンドルの指す位置にシェーダーリソースビュー作成
	DirectX12::Get()->mDevice->CreateShaderResourceView(mTexBuff.Get(), &mSrvDesc, mCpuHandle);

	mGetResDesc = textureResourceDesc;

	mCreated = true;
}

void TextureManager::Initialize()
{
	DirectX12 *dx12 = DirectX12::Get();

	//デスクリプタヒープの設定
	mSrvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	mSrvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダーから見えるように
	mSrvHeapDesc.NumDescriptors = K_MAX_SRV_COUNT;

	//設定を元にSRV用デスクリプタヒープを生成
	sResult = dx12->mDevice->CreateDescriptorHeap(&mSrvHeapDesc, IID_PPV_ARGS(&mSrvHeap));
	assert(SUCCEEDED(sResult));
}

void TextureManager::PreLoad()
{
	bugfix_.CreateWhiteTexture();

	Load("Resources\\white.png", "white");
	Load("Resources\\slime.png", "slime");
	Load("Resources\\default.png", "default");
	Load("Resources\\particle.png", "particle");
	Load("Resources\\firewisp\\tex.png", "firewisp");
	Load("Resources\\redScreen.png", "redScreen");
	Load("Resources\\ddsconvert.dds", "hogeta");
	Load("Resources\\renga.png", "renga");
	Load("Resources\\snow.png", "snow");
	Load("Resources\\ice.png", "ice");
}

//ネットで拾ってきたstring->wstring変換
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
	if (sTextures[handle].mCreated == false)
	{
		return nullptr;
	}
	return &sTextures[handle];
}
