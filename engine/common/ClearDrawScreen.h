#pragma once
#include "Pipeline.h"

void CreateDepthView();

void ClearDrawScreen();

void PreDraw();

//ライト使いたくないときもあるので、応急処理でライトを使わない設定にできるように
void BasicObjectPreDraw(const PipelineSet& objectPipelineSet,bool useLight = true);

void GeometryObjectPreDraw(const PipelineSet& geometryPipelineSet);

void PostDraw();

/*! Screen
	デプスバッファを管理するクラス
	名前適切ではないかも
*/
class Screen
{
public:
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeap = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> depthBuff;

	static Screen* Get() {
		static Screen instance;
		return &instance;
	}

private:
	Screen() {};
	~Screen() {};
};