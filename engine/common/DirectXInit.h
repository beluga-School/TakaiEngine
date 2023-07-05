#pragma once
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include "WinAPI.h"
#include <vector>
#include <string>
//�����ɒu���Ƃ��Ɗ�Ȃ��񂶂�Ȃ��H
#include<wrl.h>

//�A�_�v�^�[�̗񋓗p
static std::vector<Microsoft::WRL::ComPtr<IDXGIAdapter4>> sAdapters;

//�����ɓ���̖��O�����A�_�v�^�[�I�u�W�F�N�g������
static IDXGIAdapter4* sTmpAdapter = nullptr;

class DirectX12
{
public:

	Microsoft::WRL::ComPtr<ID3D12Device> mDevice;
	Microsoft::WRL::ComPtr<IDXGIFactory7> mDxgifactory;
	Microsoft::WRL::ComPtr<IDXGISwapChain4> mSwapChain;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mCommandAllocator;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCmdList;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> mCmdQueue;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mRtvHeap;

	DXGI_SWAP_CHAIN_DESC1 mSwapChainDesc{};
	D3D12_DESCRIPTOR_HEAP_DESC mRtvHeapDesc{};
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> mBackBuffers;

	Microsoft::WRL::ComPtr<ID3D12Fence> mFence;
	UINT64 mFenceVal = 0;

	static DirectX12* Get();

private:
	//�N�����̈�A�̗�������s
	void Execution();

	void AdapterSort();

	void CreateDevice();

	void CreateCmdList();

	void SetSwapChain();

	void SetDescHeap();

	void SetBackBuffer();

	void RenderTargetView();

	void CreateFence();

private:
	DirectX12()
	{
		Execution();
	}
	~DirectX12(){};

	DirectX12(const DirectX12& a) = delete;
	DirectX12& operator=(const DirectX12&) = delete;
};