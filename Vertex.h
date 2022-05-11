#pragma once

#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")
#include <DirectXTex.h>

#include "DirectXInit.h"

//頂点データ構造体
struct Vertex
{
	XMFLOAT3 pos;	//xyz座標
	XMFLOAT2 uv;	//uv座標
};

class VertexData
{
public:
	UINT sizeVB;
	D3D12_RESOURCE_DESC resDesc{};
	D3D12_INDEX_BUFFER_VIEW ibView{};
	//頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	void CreateVertex(DirectXInit dx12_);
};

//頂点データ
static Vertex vertices[] = {
	// x	y	  z		 u	   v
	{{-0.4f,-0.7f,0.0f},{0.0f,1.0f}},	//左下
	{{-0.4f,+0.7f,0.0f},{0.0f,0.0f}},	//左上
	{{+0.4f,-0.7f,0.0f},{1.0f,1.0f}},	//右下
	{{+0.4f,+0.7f,0.0f},{1.0f,0.0f}},	//右上
};

//インデックスデータ
static uint16_t indices[] =
{
	0,1,2,//三角形1つ目
	1,2,3,//三角形2つ目
};
