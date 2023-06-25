#pragma once
#include "Sprite.h"

class PostEffect 
{
public:
	PostEffect();

	void Initialize();

	void Update();

	void Draw();

	void PreDrawScene();

	void PostDrawScene();

	//パイプライン切り替え用にとりあえず置いておく
	std::string pipeLineName = "None";

private:
	//画面クリア用の色
	static const float sClearColor[4];

	//テクスチャバッファ
	ComPtr<ID3D12Resource> mTexBuff[2];
	//SRV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> mDescHeapSRV;
	//深度バッファ
	ComPtr<ID3D12Resource> mDepthBuff;
	//RTV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> mDescHeapRTV;
	//DSV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> mDescHeapDSV;

	//頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> mVertBuff;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW mVbView{};
	//定数バッファ
	ConstBuffer<ConstBufferDataSprite> mConstBuffer;

	//まとめ関数
	//テクスチャ生成(今の画面からテクスチャを生成？)
	void CreateTexture();

	//SRV(シェーダーリソースビュー)作成
	void CreateSRV();

	//RTV(レンダーターゲットビュー)作成
	void CreateRTV();
	
	//深度バッファ生成
	void CreateDepthBuff();

	//DSV(デプスステンシルビュー)作成
	void CreateDSV();
};