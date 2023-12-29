#pragma once
#include "Sprite.h"

struct ConstBufferTime
{
	float time;
};

/*! PostEffect
	ポストエフェクト用の事前描画と描画後処理を持つクラス
*/
class PostEffect 
{
public:
	PostEffect();

	void Initialize();

	void Update();

	void Draw();

	//ポストエフェクト描画に対応させるための前準備
	void PreDrawScene();

	//ポストエフェクト描画に対応させるための後処理
	void PostDrawScene();

private:
	//まとめ関数
	//テクスチャ生成(今の画面からテクスチャを生成する)
	void CreateTexture();

	//SRV(シェーダーリソースビュー)作成
	void CreateSRV();

	//RTV(レンダーターゲットビュー)作成
	void CreateRTV();

	//深度バッファ生成
	void CreateDepthBuff();

	//DSV(デプスステンシルビュー)作成
	void CreateDSV();

public:
	//パイプライン切り替え用にとりあえず置いておく
	static std::string pipeLineName;

private:
	//画面クリア用の色
	static const float sClearColor[4];

	//テクスチャバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> mTexBuff[2];
	//SRV用デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mDescHeapSRV;
	//深度バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> mDepthBuff;
	//RTV用デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mDescHeapRTV;
	//DSV用デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mDescHeapDSV;

	//頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> mVertBuff;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW mVbView{};
	//定数バッファ
	ConstBuffer<ConstBufferTime> mConstBuffer;
};