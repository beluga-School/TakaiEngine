#include "PostEffectt.h"
#include "util.h"
#include "Result.h"
#include <memory>
#include "ClearDrawScreen.h"

const float PostEffect::sClearColor[4] = { 0.25f,0.5f,0.1f,0.0f };//緑っぽい色でクリア

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
	CreateTexture();
	
	CreateSRV();

	CreateRTV();

	CreateDepthBuff();

	CreateDSV();	
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

	//スプライトのパイプラインステートを使いまわしてる
	// CULLMODE=NONE、DepthFunc=ALWAYS で描画している
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

void PostEffect::PreDrawScene()
{
	DirectX12* dx12 = DirectX12::Get();

	D3D12_RESOURCE_BARRIER barrierDesc{};

	//テクスチャを、画面全体を描画するときと同じように書き込む為の準備
	barrierDesc.Transition.pResource = mTexBuff.Get();
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;		//表示状態から
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;	//描画状態へ
	dx12->mCmdList->ResourceBarrier(1, &barrierDesc);

	D3D12_CPU_DESCRIPTOR_HANDLE rtvH =
		mDescHeapRTV->GetCPUDescriptorHandleForHeapStart();

	D3D12_CPU_DESCRIPTOR_HANDLE dsvH =
		mDescHeapDSV->GetCPUDescriptorHandleForHeapStart();

	dx12->mCmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);

	D3D12_VIEWPORT viewport{};
	viewport.Width = Util::WIN_WIDTH;
	viewport.Height = Util::WIN_HEIGHT;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	dx12->mCmdList->RSSetViewports(1, &viewport);

	D3D12_RECT scissorRect{};
	scissorRect.left = 0;									
	scissorRect.right = scissorRect.left + Util::WIN_WIDTH;	
	scissorRect.top = 0;									
	scissorRect.bottom = scissorRect.top + Util::WIN_HEIGHT;
	dx12->mCmdList->RSSetScissorRects(1, &scissorRect);

	//全画面クリア
	dx12->mCmdList->ClearRenderTargetView(rtvH, sClearColor, 0, nullptr);

	//深度バッファのクリア
	dx12->mCmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void PostEffect::PostDrawScene()
{
	DirectX12* dx12 = DirectX12::Get();
	Screen* screen = Screen::Get();

	//リソースバリアの状態を戻す
	D3D12_RESOURCE_BARRIER barrierDesc{};
	barrierDesc.Transition.pResource = mTexBuff.Get();
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

	dx12->mCmdList->ResourceBarrier(1, &barrierDesc);

	//レンダーターゲットを戻す
	UINT bbIndex = dx12->mSwapChain->GetCurrentBackBufferIndex();

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = dx12->mRtvHeap->GetCPUDescriptorHandleForHeapStart();
	rtvHandle.ptr += bbIndex * dx12->mDevice->GetDescriptorHandleIncrementSize(dx12->mRtvHeapDesc.Type);

	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = screen->dsvHeap->GetCPUDescriptorHandleForHeapStart();
	dx12->mCmdList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

}

void PostEffect::CreateTexture()
{
	DirectX12* dx12 = DirectX12::Get();

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

	D3D12_CLEAR_VALUE texClearValue{};
	for (int32_t i = 0; i < 4; i++)
	{
		texClearValue.Color[i] = sClearColor[i];
	}
	texClearValue.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	//テクスチャバッファの生成
	sResult = dx12->mDevice->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		&texClearValue,
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
}

void PostEffect::CreateSRV()
{
	DirectX12* dx12 = DirectX12::Get();

	///--新しいSRV用デスクリプタヒープを作成
	//SRV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeaoDesc = {};

	//デスクリプタヒープの設定
	srvDescHeaoDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeaoDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダーから見えるように
	srvDescHeaoDesc.NumDescriptors = 1; //レンダーテクスチャ専用のヒープなため、サイズは1でいい

	//設定を元にSRV用デスクリプタヒープを生成
	sResult = DirectX12::Get()->mDevice->CreateDescriptorHeap(&srvDescHeaoDesc, IID_PPV_ARGS(&mDescHeapSRV));
	assert(SUCCEEDED(sResult));

	//シェーダリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC mSrvDesc{};
	mSrvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	mSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	mSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	mSrvDesc.Texture2D.MipLevels = 1;

	//ハンドルの指す位置にシェーダーリソースビュー作成
	DirectX12::Get()->mDevice->CreateShaderResourceView(mTexBuff.Get(), &mSrvDesc, mDescHeapSRV->GetCPUDescriptorHandleForHeapStart());

}

void PostEffect::CreateRTV()
{
	DirectX12* dx12 = DirectX12::Get();

	///--レンダーテクスチャを作成
	//画面描画のためのバックバッファと同じ

	//RTV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};
	rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescHeapDesc.NumDescriptors = 1;
	//RTV用デスクリプタヒープを生成
	sResult = dx12->mDevice->CreateDescriptorHeap(
		&rtvDescHeapDesc,
		IID_PPV_ARGS(&mDescHeapRTV));
	assert(SUCCEEDED(sResult));

	//レンダーターゲットビューの設定
	D3D12_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};
	//シェーダーの計算結果をSRGBに変換して書き込む
	renderTargetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	renderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	//デスクリプタヒープにRTV作成
	dx12->mDevice->CreateRenderTargetView(
		mTexBuff.Get(),
		&renderTargetViewDesc,
		mDescHeapRTV->GetCPUDescriptorHandleForHeapStart()
	);
}

void PostEffect::CreateDepthBuff()
{
	DirectX12* dx12 = DirectX12::Get();

	//深度バッファリソース設定
	D3D12_RESOURCE_DESC depthResourceDesc{};
	depthResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResourceDesc.Width = Util::WIN_WIDTH;	//レンダーターゲットに合わせる
	depthResourceDesc.Height = Util::WIN_HEIGHT;	//レンダーターゲットに合わせる
	depthResourceDesc.DepthOrArraySize = 1;
	depthResourceDesc.Format = DXGI_FORMAT_D32_FLOAT;	//深度値フォーマット
	depthResourceDesc.SampleDesc.Count = 1;
	depthResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	//深度値用ヒーププロパティ
	D3D12_HEAP_PROPERTIES depthHeapProp{};
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;

	//深度値のクリア設定
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f;	//深度値1.0f(最大値)でクリア
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;	//深度値フォーマット

	//リソース生成
	sResult = dx12->mDevice->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,	//深度値書き込みに使用
		&depthClearValue,
		IID_PPV_ARGS(&mDepthBuff));
	assert(SUCCEEDED(sResult));
}

void PostEffect::CreateDSV()
{
	DirectX12* dx12 = DirectX12::Get();

	//DSV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	descHeapDesc.NumDescriptors = 1;

	//DSV用デスクリプタヒープを作成
	sResult = dx12->mDevice->CreateDescriptorHeap(
		&descHeapDesc,
		IID_PPV_ARGS(&mDescHeapDSV)
	);

	assert(SUCCEEDED(sResult));

	//深度ビュー作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;//深度値フォーマット
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dx12->mDevice->CreateDepthStencilView(
		mDepthBuff.Get(),
		&dsvDesc,
		mDescHeapDSV->GetCPUDescriptorHandleForHeapStart());
}
