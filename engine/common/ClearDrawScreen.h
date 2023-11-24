#pragma once
#include "Pipeline.h"

//デプスビューを設定
void CreateDepthView();

//スクリーンを初期化する
void ClearDrawScreen();

//描画の前準備
void PreDraw();

//ライト使いたくないときもあるので、応急処理でライトを使わない設定にできるように
void BasicObjectPreDraw(const std::string& pipelineName,bool useLight = true);

//ジオメトリシェーダー
void GeometryObjectPreDraw(const std::string& pipelineName);

//描画後の処理
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