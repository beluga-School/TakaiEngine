#pragma once
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <DirectXMath.h>
using namespace DirectX;

//頂点データ
XMFLOAT3 vertices[] = {
   { -0.5f,-0.5f,0.0f},//左下
   { -0.5f,+0.5f,0.0f},//左上
   { +0.5f,-0.5f,0.0f},//右下
};

class DrawInit
{
public:

	//頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(XMFLOAT3) * _countof(vertices));

	//頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapProp{};		//ヒープ設定

	//リソース設定
	D3D12_RESOURCE_DESC resDesc{};

	DrawInit();
	~DrawInit();

	void SetResDesc();
};

