#pragma once
#include "Pipeline.h"

void CreateDepthView();

void ClearDrawScreen();

void PreDraw();

void BasicObjectPreDraw(const PipelineSet& objectPipelineSet);

void GeometryObjectPreDraw(const PipelineSet& geometryPipelineSet);

void PostDraw();

class Screen
{
public:
	ComPtr<ID3D12DescriptorHeap> dsvHeap = nullptr;

	ComPtr<ID3D12Resource> depthBuff;

	//D3D12_RESOURCE_BARRIER barrierDesc{};

	static Screen* Get() {
		static Screen instance;
		return &instance;
	}

private:
	Screen() {};
	~Screen() {};
};