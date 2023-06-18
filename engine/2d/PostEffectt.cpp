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

	//プログラムで作成したテクスチャを貼る練習
	//texturemanagerでwhiteテクスチャを生成した手順と同じようなことをすればいい
	//資料見てもうまくいかなかったので、他の人のやつをみる

	//mTEXTUREの部分を置き換えられればいい

	//リソース設定
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	textureResourceDesc.Width = Util::WIN_WIDTH;
	textureResourceDesc.Height = (UINT)Util::WIN_HEIGHT;
	textureResourceDesc.DepthOrArraySize = 1;
	textureResourceDesc.MipLevels = 1;
	textureResourceDesc.SampleDesc.Count = 1;
	textureResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

	//ヒープ設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty =
		D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	//テクスチャバッファの生成
	sResult = dx12->mDevice->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		nullptr,
		IID_PPV_ARGS(&mTexBuff));
	assert(SUCCEEDED(sResult));

	{//テクスチャを赤でクリア
		
		//縦横のピクセル分の画素数
		const UINT pixelCount = Util::WIN_WIDTH * Util::WIN_HEIGHT;

		const UINT rowPitch = sizeof(UINT) * Util::WIN_WIDTH;

		const UINT depthPitch = rowPitch * Util::WIN_HEIGHT;

		UINT* img = new UINT[pixelCount];
		for (int32_t i = 0; i < pixelCount; i++)
		{
			img[i] = 0xff0000ff;
		}

		//テクスチャバッファにデータ転送
		sResult = mTexBuff->WriteToSubresource(
			0, nullptr,
			img, rowPitch, depthPitch
		);
		assert(SUCCEEDED(sResult));
		delete[] img;
	}

	///--新しいSRV用デスクリプタヒープを作成
	//SRV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeaoDesc = {};
	
	//デスクリプタヒープの設定
	srvDescHeaoDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeaoDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダーから見えるように
	srvDescHeaoDesc.NumDescriptors = 1; //レンダーテクスチャ専用のヒープなため、サイズは1でいい

	//設定を元にSRV用デスクリプタヒープを生成
	sResult = dx12->mDevice->CreateDescriptorHeap(&srvDescHeaoDesc, IID_PPV_ARGS(&mDescHeapSRV));
	assert(SUCCEEDED(sResult));

	//シェーダリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC mSrvDesc{};
	mSrvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	mSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	mSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	mSrvDesc.Texture2D.MipLevels = 1;

	//ハンドルの指す位置にシェーダーリソースビュー作成
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

	//パイプラインステートの設定
	dx12->mCmdList->SetPipelineState(SpriteCommon::sSpriteCommon.mPipelineSet.mPipelinestate.Get());
	//ルートシグネチャの設定
	dx12->mCmdList->SetGraphicsRootSignature(SpriteCommon::sSpriteCommon.mPipelineSet.mRootsignature.Get());
	//プリミティブ形状を設定
	dx12->mCmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//GetAddressOfしないとバグります 理由は忘れちゃった ComPtrだからだっけ？
	dx12->mCmdList->SetDescriptorHeaps(1, mDescHeapSRV.GetAddressOf());

	dx12->mCmdList->IASetVertexBuffers(0, 1, &mVbView);

	dx12->mCmdList->SetGraphicsRootDescriptorTable(1, mDescHeapSRV->GetGPUDescriptorHandleForHeapStart());

	dx12->mCmdList->SetGraphicsRootConstantBufferView(0, mConstBuffer.mBuffer->GetGPUVirtualAddress());

	dx12->mCmdList->DrawInstanced(4, 1, 0, 0);
}
