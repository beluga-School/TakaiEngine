#pragma once
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include "WinAPI.h"
#include <vector>
#include <string>
//ここに置いとくと危ないんじゃない？
#include<wrl.h>
using namespace Microsoft::WRL;

//アダプターの列挙用
static std::vector<ComPtr<IDXGIAdapter4>> sAdapters;

//ここに特定の名前をもつアダプターオブジェクトが入る
static IDXGIAdapter4* sTmpAdapter = nullptr;

class DirectX12
{
public:

	ComPtr<ID3D12Device> mDevice;
	ComPtr<IDXGIFactory7> mDxgifactory;
	ComPtr<IDXGISwapChain4> mSwapChain;
	ComPtr<ID3D12CommandAllocator> mCommandAllocator;
	ComPtr<ID3D12GraphicsCommandList> mCmdList;
	ComPtr<ID3D12CommandQueue> mCmdQueue;
	ComPtr<ID3D12DescriptorHeap> mRtvHeap;

	DXGI_SWAP_CHAIN_DESC1 mSwapChainDesc{};
	D3D12_DESCRIPTOR_HEAP_DESC mRtvHeapDesc{};
	std::vector<ComPtr<ID3D12Resource>> mBackBuffers;

	ComPtr<ID3D12Fence> mFence;
	UINT64 mFenceVal = 0;

	static DirectX12* Get();

private:
	//起動時の一連の流れを実行
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