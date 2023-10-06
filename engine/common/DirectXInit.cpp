#include "DirectXInit.h"
#include "Result.h"

void DirectX12::Execution()
{
	AdapterSort();

	CreateDevice();

	CreateCmdList();

	SetSwapChain();

	SetDescHeap();

	SetBackBuffer();

	RenderTargetView();

	CreateFence();
}

void DirectX12::AdapterSort()
{
	///---グラフィックボードのアダプタを列挙---///
	//DXGIファクトリーの生成
	sResult = CreateDXGIFactory(IID_PPV_ARGS(&mDxgifactory));

	//パフォーマンスが高いものから順に、すべてのアダプターを列挙する
	for (UINT i = 0;
		mDxgifactory->EnumAdapterByGpuPreference(i,
			DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
			IID_PPV_ARGS(&sTmpAdapter)) != DXGI_ERROR_NOT_FOUND;
		i++)
	{
		//動的配列に追加する
		sAdapters.push_back(sTmpAdapter);
	}
	///------///

	///---グラフィックスボードのアダプタを選別---///
	for (size_t i = 0; i < sAdapters.size(); i++)
	{
		DXGI_ADAPTER_DESC3 adapterDesc;

		//アダプターの情報を取得する
		sAdapters[i]->GetDesc3(&adapterDesc);

		//ソフトウェアデバイスを回避
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {
			//デバイスを採用してループを抜ける
			sTmpAdapter = sAdapters[i].Get();
			break;
		}
	}
	///------///
}

void DirectX12::CreateDevice()
{
	///---デバイスの生成---///

	D3D_FEATURE_LEVEL levels[] = {
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	D3D_FEATURE_LEVEL featureLevel;

	for (size_t i = 0; i < _countof(levels); i++)
	{
		//採用したアダプターでデバイスを生成
		sResult = D3D12CreateDevice(sTmpAdapter, levels[i],
			IID_PPV_ARGS(&mDevice));
		if (sResult == S_OK) {
			featureLevel = levels[i];
			break;
		}
	}

	///------///
}

void DirectX12::CreateCmdList()
{
	//コマンドアロケータを生成
	sResult = mDevice->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&mCommandAllocator));
	assert(SUCCEEDED(sResult));

	//コマンドリストを生成
	sResult = mDevice->CreateCommandList(0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		mCommandAllocator.Get(), nullptr,
		IID_PPV_ARGS(&mCmdList));
	assert(SUCCEEDED(sResult));

	//コマンドキューの設定
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
	//コマンドキューを生成
	sResult = mDevice->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&mCmdQueue));
	assert(SUCCEEDED(sResult));
}

void DirectX12::SetSwapChain()
{
	//スワップチェーンの設定
	mSwapChainDesc.Width = 1280;
	mSwapChainDesc.Height = 720;
	mSwapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	//色情報の書式
	mSwapChainDesc.SampleDesc.Count = 1;					//マルチサンプルしない
	mSwapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;	//バックバッファ用
	mSwapChainDesc.BufferCount = 2;						//バッファ数を2つに設定
	mSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;	//フリップ後は破棄
	mSwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	//ComPtrに対応
	Microsoft::WRL::ComPtr<IDXGISwapChain1> swapchain1;

	//スワップチェーンの生成
	sResult = mDxgifactory->CreateSwapChainForHwnd(
		mCmdQueue.Get(), 
		WinAPI::Get()->mHwnd,
		&mSwapChainDesc, 
		nullptr, 
		nullptr,
		(IDXGISwapChain1**)&swapchain1);
	assert(SUCCEEDED(sResult));

	swapchain1.As(&mSwapChain);
}

void DirectX12::SetDescHeap()
{
	//デスクリプタヒープの設定
	mRtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;		//レンダーターゲットビュー
	mRtvHeapDesc.NumDescriptors = mSwapChainDesc.BufferCount;	//裏表の2つ

	//デスクリプタヒープの生成
	mDevice->CreateDescriptorHeap(&mRtvHeapDesc, IID_PPV_ARGS(&mRtvHeap));

}

void DirectX12::SetBackBuffer()
{
	//バックバッファ
	mBackBuffers.resize(mSwapChainDesc.BufferCount);
}

void DirectX12::RenderTargetView()
{
	//スワップチェーンのすべてのバッファについて処理する
	for (size_t i = 0; i < mBackBuffers.size(); i++)
	{
		//スワップチェーンからバッファを取得
		mSwapChain->GetBuffer((UINT)i, IID_PPV_ARGS(&mBackBuffers[i]));

		//デスクリプタヒープのハンドルを取得
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = mRtvHeap->GetCPUDescriptorHandleForHeapStart();

		//裏か表かでアドレスがずれる
		rtvHandle.ptr += i * mDevice->GetDescriptorHandleIncrementSize(mRtvHeapDesc.Type);

		//レンダーターゲットビューの設定
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};

		//シェーダーの計算結果をSRGBに変換して書き込む
		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

		//レンダーターゲットビューの生成
		mDevice->CreateRenderTargetView(mBackBuffers[i].Get(), &rtvDesc, rtvHandle);

	}
}

void DirectX12::CreateFence()
{
	//フェンスの生成
	sResult = mDevice->CreateFence(mFenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence));
}

DirectX12* DirectX12::Get()
{
	static DirectX12 instance;
	return &instance;
}