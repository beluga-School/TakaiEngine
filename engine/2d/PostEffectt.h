#pragma once
#include "Sprite.h"

class PostEffect :public Sprite
{
public:
	PostEffect();

	void Initialize();

	void Draw();

private:
	//テクスチャバッファ
	ComPtr<ID3D12Resource> mTexBuff;
	//SRV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> mDescHeapSRV;
};