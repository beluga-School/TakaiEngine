#pragma once
#include "Sprite.h"

class PostEffect : public Sprite
{
public:
	PostEffect();

	void Initialize();

	void Draw();

	void PreDrawScene();

	void PostDrawScene();

private:
	//画面クリア用の色
	static const float sClearColor[4];

	//テクスチャバッファ
	ComPtr<ID3D12Resource> mTexBuff;
	//SRV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> mDescHeapSRV;
	//深度バッファ
	ComPtr<ID3D12Resource> mDepthBuff;
	//RTV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> mDescHeapRTV;
	//DSV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> mDescHeapDSV;

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