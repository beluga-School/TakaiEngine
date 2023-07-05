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
	///---�O���t�B�b�N�{�[�h�̃A�_�v�^���---///
	//DXGI�t�@�N�g���[�̐���
	sResult = CreateDXGIFactory(IID_PPV_ARGS(&mDxgifactory));

	//�p�t�H�[�}���X���������̂��珇�ɁA���ׂẴA�_�v�^�[��񋓂���
	for (UINT i = 0;
		mDxgifactory->EnumAdapterByGpuPreference(i,
			DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
			IID_PPV_ARGS(&sTmpAdapter)) != DXGI_ERROR_NOT_FOUND;
		i++)
	{
		//���I�z��ɒǉ�����
		sAdapters.push_back(sTmpAdapter);
	}
	///------///

	///---�O���t�B�b�N�X�{�[�h�̃A�_�v�^��I��---///
	for (size_t i = 0; i < sAdapters.size(); i++)
	{
		DXGI_ADAPTER_DESC3 adapterDesc;

		//�A�_�v�^�[�̏����擾����
		sAdapters[i]->GetDesc3(&adapterDesc);

		//�\�t�g�E�F�A�f�o�C�X�����
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {
			//�f�o�C�X���̗p���ă��[�v�𔲂���
			sTmpAdapter = sAdapters[i].Get();
			break;
		}
	}
	///------///
}

void DirectX12::CreateDevice()
{
	///---�f�o�C�X�̐���---///

	D3D_FEATURE_LEVEL levels[] = {
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	D3D_FEATURE_LEVEL featureLevel;

	for (size_t i = 0; i < _countof(levels); i++)
	{
		//�̗p�����A�_�v�^�[�Ńf�o�C�X�𐶐�
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
	//�R�}���h�A���P�[�^�𐶐�
	sResult = mDevice->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&mCommandAllocator));
	assert(SUCCEEDED(sResult));

	//�R�}���h���X�g�𐶐�
	sResult = mDevice->CreateCommandList(0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		mCommandAllocator.Get(), nullptr,
		IID_PPV_ARGS(&mCmdList));
	assert(SUCCEEDED(sResult));

	//�R�}���h�L���[�̐ݒ�
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
	//�R�}���h�L���[�𐶐�
	sResult = mDevice->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&mCmdQueue));
	assert(SUCCEEDED(sResult));
}

void DirectX12::SetSwapChain()
{
	//�X���b�v�`�F�[���̐ݒ�
	mSwapChainDesc.Width = 1280;
	mSwapChainDesc.Height = 720;
	mSwapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	//�F���̏���
	mSwapChainDesc.SampleDesc.Count = 1;					//�}���`�T���v�����Ȃ�
	mSwapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;	//�o�b�N�o�b�t�@�p
	mSwapChainDesc.BufferCount = 2;						//�o�b�t�@����2�ɐݒ�
	mSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;	//�t���b�v��͔j��
	mSwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	//ComPtr�ɑΉ�
	Microsoft::WRL::ComPtr<IDXGISwapChain1> swapchain1;

	//�X���b�v�`�F�[���̐���
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
	//�f�X�N���v�^�q�[�v�̐ݒ�
	mRtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;		//�����_�[�^�[�Q�b�g�r���[
	mRtvHeapDesc.NumDescriptors = mSwapChainDesc.BufferCount;	//���\��2��

	//�f�X�N���v�^�q�[�v�̐���
	mDevice->CreateDescriptorHeap(&mRtvHeapDesc, IID_PPV_ARGS(&mRtvHeap));

}

void DirectX12::SetBackBuffer()
{
	//�o�b�N�o�b�t�@
	mBackBuffers.resize(mSwapChainDesc.BufferCount);
}

void DirectX12::RenderTargetView()
{
	//�X���b�v�`�F�[���̂��ׂẴo�b�t�@�ɂ��ď�������
	for (size_t i = 0; i < mBackBuffers.size(); i++)
	{
		//�X���b�v�`�F�[������o�b�t�@���擾
		mSwapChain->GetBuffer((UINT)i, IID_PPV_ARGS(&mBackBuffers[i]));

		//�f�X�N���v�^�q�[�v�̃n���h�����擾
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = mRtvHeap->GetCPUDescriptorHandleForHeapStart();

		//�����\���ŃA�h���X�������
		rtvHandle.ptr += i * mDevice->GetDescriptorHandleIncrementSize(mRtvHeapDesc.Type);

		//�����_�[�^�[�Q�b�g�r���[�̐ݒ�
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};

		//�V�F�[�_�[�̌v�Z���ʂ�SRGB�ɕϊ����ď�������
		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

		//�����_�[�^�[�Q�b�g�r���[�̐���
		mDevice->CreateRenderTargetView(mBackBuffers[i].Get(), &rtvDesc, rtvHandle);

	}
}

void DirectX12::CreateFence()
{
	//�t�F���X�̐���
	sResult = mDevice->CreateFence(mFenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence));
}

DirectX12* DirectX12::Get()
{
	static DirectX12 instance;
	return &instance;
}