#pragma once
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include "WinAPI.h"
#include <vector>
#include <string>

//アダプターの列挙用
static std::vector<IDXGIAdapter4*> adapters;

//ここに特定の名前をもつアダプターオブジェクトが入る
static IDXGIAdapter4* tmpAdapter = nullptr;

class DirectX12
{
public:

	ID3D12Device* device = nullptr;
	IDXGIFactory7* dxgifactory = nullptr;
	IDXGISwapChain4* swapChain = nullptr;
	ID3D12CommandAllocator* commandAllocator = nullptr;
	ID3D12GraphicsCommandList* commandList = nullptr;
	ID3D12CommandQueue* commandQueue = nullptr;
	ID3D12DescriptorHeap* rtvHeap = nullptr;

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	std::vector<ID3D12Resource*> backBuffers;

	ID3D12Fence* fence = nullptr;
	UINT64 fenceVal = 0;

public:
	void yobidasi(WinAPI winApi_);

	void AdapterSort();

	void CreateDevice();

	void CreateCmdList();

	void SetSwapChain(WinAPI winApi_);

	void SetDescHeap();

	void SetBackBuffer();

	void RenderTargetView();

	void CreateFence();
};

static DirectX12 DX12;

ID3D12Device *bGetDevice();