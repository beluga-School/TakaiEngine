#pragma once
#include "Pipeline.h"

void CreateDepthView();

void ClearDrawScreen();

void PreDraw();

//���C�g�g�������Ȃ��Ƃ�������̂ŁA���}�����Ń��C�g���g��Ȃ��ݒ�ɂł���悤��
void BasicObjectPreDraw(const PipelineSet& objectPipelineSet,bool useLight = true);

void GeometryObjectPreDraw(const PipelineSet& geometryPipelineSet);

void PostDraw();

class Screen
{
public:
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeap = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> depthBuff;

	//D3D12_RESOURCE_BARRIER barrierDesc{};

	static Screen* Get() {
		static Screen instance;
		return &instance;
	}

private:
	Screen() {};
	~Screen() {};
};