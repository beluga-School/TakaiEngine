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

	//リソース設定
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//ヒープ設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty =
		D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	//リソース設定
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureResourceDesc.Width = textureWidth;
	textureResourceDesc.Height = textureHeight;
	textureResourceDesc.DepthOrArraySize = 1;
	textureResourceDesc.MipLevels = 1;
	textureResourceDesc.SampleDesc.Count = 1;

	//テクスチャバッファの生成
	result = dx12->device->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff));

	//テクスチャバッファにデータ転送
	result = texBuff->WriteToSubresource(
		0,
		nullptr,	//全領域へコピー
		imageData.get(),	//元データアドレス
		sizeof(XMFLOAT4) * textureWidth,	//1ラインサイズ
		sizeof(XMFLOAT4) * imageDatacount	//全サイズ
	);
	assert(SUCCEEDED(result));

	TextureManager* tManager = TextureManager::GetInstance();

	//ハンドルを取得する部分
	//gpuハンドルを取得
	gpuHandle = tManager->srvHeap->GetGPUDescriptorHandleForHeapStart();

	//cpuハンドルを取得
	cpuHandle = tManager->srvHeap->GetCPUDescriptorHandleForHeapStart();

	//前回のSRVヒープの大きさを加算
	cpuHandle.ptr += tManager->SRVHandleSize;

	gpuHandle.ptr += tManager->SRVHandleSize;

	//SRVヒープの大きさを取得
	tManager->SRVHandleSize += dx12->device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//シェーダリソースビュー設定
	srvDesc.Format = resDesc.Format;//RGBA float
	srvDesc.Shader4ComponentMapping =
		D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = resDesc.MipLevels;

	//ハンドルの指す位置にシェーダーリソースビュー作成
	dx12->device->CreateShaderResourceView(texBuff.Get(), &srvDesc, cpuHandle);

	getResDesc = textureResourceDesc;
}

void Texture::Load(const wchar_t* t)
{
	//リソース設定
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	result = LoadFromWICFile(
		t,	//ここに読み込みたいファイルのパスを入れる
		WIC_FLAGS_NONE,
		&metadata, scratchImg
	);

	//ミップマップ生成
	result = GenerateMipMaps(
		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain);
	if (SUCCEEDED(result)) {
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	//読み込んだディフューズテクスチャをSRGBとして扱う
	metadata.format = MakeSRGB(metadata.format);

	//ヒープ設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty =
		D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	//リソース設定
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = metadata.format;
	textureResourceDesc.Width = metadata.width;
	textureResourceDesc.Height = (UINT)metadata.height;
	textureResourceDesc.DepthOrArraySize = (UINT)metadata.arraySize;
	textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResourceDesc.SampleDesc.Count = 1;

	//テクスチャバッファの生成
	result = dx12->device->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff));

	for (SIZE_T j = 0; j < metadata.mipLevels; j++)
	{
		//ミップマップレベルを指定してイメージを取得
		const Image* img = scratchImg.GetImage(j, 0, 0);
		//テクスチャバッファにデータ転送
		result = texBuff->WriteToSubresource(
			(UINT)j,
			nullptr,	//全領域へコピー
			img->pixels,	//元データアドレス
			(UINT)img->rowPitch,	//1ラインサイズ
			(UINT)img->slicePitch	//全サイズ
		);
		assert(SUCCEEDED(result));
	}

	TextureManager *tManager = TextureManager::GetInstance();

	//ハンドルを取得する部分
	//gpuハンドルを取得
	gpuHandle = tManager->srvHeap->GetGPUDescriptorHandleForHeapStart();

	//cpuハンドルを取得
	cpuHandle = tManager->srvHeap->GetCPUDescriptorHandleForHeapStart();

	//SRVヒープの大きさを加算
	cpuHandle.ptr += tManager->SRVHandleSize;

	gpuHandle.ptr += tManager->SRVHandleSize;

	//SRVヒープの大きさを取得
	tManager->SRVHandleSize += dx12->device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//シェーダリソースビュー設定
	srvDesc.Format = resDesc.Format;//RGBA float
	srvDesc.Shader4ComponentMapping =
		D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = resDesc.MipLevels;

	//ハンドルの指す位置にシェーダーリソースビュー作成
	dx12->device->CreateShaderResourceView(texBuff.Get(), &srvDesc, cpuHandle);

	getResDesc = textureResourceDesc;
}

void TextureManager::Initialize()
{
	DirectX12 *dx12 = DirectX12::GetInstance();

	//デスクリプタヒープの設定
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダーから見えるように
	srvHeapDesc.NumDescriptors = kMaxSRVCount;

	//設定を元にSRV用デスクリプタヒープを生成
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
