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
#include <vector>

std::vector<int>;

//頂点データ構造体
struct Vertex
{
	XMFLOAT3 pos;	//xyz座標
	XMFLOAT3 normal;	//法線ベクトル
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

	void CreateVertex(DirectXInit dx12_, std::vector<Vertex> vertices, std::vector<uint16_t> indices);
};

//頂点データ
//static Vertex vertices[] = {
//	// x	y	  z		 u	   v
//	{{-0.4f,-0.7f,0.0f},{0.0f,1.0f}},	//左下
//	{{-0.4f,+0.7f,0.0f},{0.0f,0.0f}},	//左上
//	{{+0.4f,-0.7f,0.0f},{1.0f,1.0f}},	//右下
//	{{+0.4f,+0.7f,0.0f},{1.0f,0.0f}},	//右上
//};

static std::vector<Vertex> vertices = {
	// x	 y	   z	 n	 u	   v
	//前
	{{-5.0f,-5.0f,-5.0f},{} ,{0.0f,1.0f}},	//左下
	{{-5.0f, 5.0f,-5.0f},{} ,{0.0f,0.0f}},	//左上
	{{ 5.0f,-5.0f,-5.0f},{} ,{1.0f,1.0f}},	//右下
	{{ 5.0f, 5.0f,-5.0f},{} ,{1.0f,0.0f}},	//右上
	//後ろ			 
	{{-5.0f,-5.0f, 5.0f},{} ,{0.0f,1.0f}},	//左下
	{{-5.0f, 5.0f, 5.0f},{} ,{0.0f,0.0f}},	//左上
	{{ 5.0f,-5.0f, 5.0f},{} ,{1.0f,1.0f}},	//右下
	{{ 5.0f, 5.0f, 5.0f},{} ,{1.0f,0.0f}},	//右上
	//左				
	{{-5.0f,-5.0f,-5.0f},{} ,{0.0f,1.0f}},	//左下
	{{-5.0f,-5.0f, 5.0f},{} ,{0.0f,0.0f}},	//左上
	{{-5.0f, 5.0f,-5.0f},{} ,{1.0f,1.0f}},	//右下
	{{-5.0f, 5.0f, 5.0f},{} ,{1.0f,0.0f}},	//右上
	//右				
	{{ 5.0f,-5.0f,-5.0f},{} ,{0.0f,1.0f}},	//左下
	{{ 5.0f,-5.0f, 5.0f},{} ,{0.0f,0.0f}},	//左上
	{{ 5.0f, 5.0f,-5.0f},{} ,{1.0f,1.0f}},	//右下
	{{ 5.0f, 5.0f, 5.0f},{} ,{1.0f,0.0f}},	//右上
	//下					
	{{-5.0f,-5.0f,-5.0f},{} ,{0.0f,1.0f}},	//左下
	{{-5.0f,-5.0f, 5.0f},{} ,{0.0f,0.0f}},	//左上
	{{ 5.0f,-5.0f,-5.0f},{} ,{1.0f,1.0f}},	//右下
	{{ 5.0f,-5.0f, 5.0f},{} ,{1.0f,0.0f}},	//右上
	//上				
	{{-5.0f, 5.0f,-5.0f},{} ,{0.0f,1.0f}},	//左下
	{{-5.0f, 5.0f, 5.0f},{} ,{0.0f,0.0f}},	//左上
	{{ 5.0f, 5.0f,-5.0f},{} ,{1.0f,1.0f}},	//右下
	{{ 5.0f, 5.0f, 5.0f},{} ,{1.0f,0.0f}},	//右上
};

//インデックスデータ
static std::vector<uint16_t> indices =
{
	//前
	0,1,2,		//三角形1つ目
	2,1,3,		//三角形2つ目
	//後ろ(前の面に4加算)
	6,5,4,		//三角形4つ目
	7,5,6,		//三角形3つ目
				//左
	8,9,10,		//三角形5つ目
	10,9,11,	//三角形6つ目
	//右
	14,13,12,	//三角形7つ目
	15,13,14,	//三角形8つ目
	//下
	19,17,18,	//三角形10つ目
	18,17,16,	//三角形9つ目
	//上
	20,21,22,	//三角形11つ目
	22,21,23,	//三角形12つ目

	//三角形を2つ合わせて四角にしてるので
	//6面にするなら12個必要...ってｺﾄ!? ﾜ｡｡｡
};
