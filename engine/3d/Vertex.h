#pragma once
#define NOMINMAX
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

#include "DirectXInit.h"
#include <vector>

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
	//UINT sizeVB;
	D3D12_RESOURCE_DESC resDesc{};
	D3D12_INDEX_BUFFER_VIEW ibView{};
	//頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	void CreateVertex(const std::vector<Vertex> &vertices,const std::vector<uint16_t> &indices);
	
	ComPtr<ID3D12Resource> vertBuff = nullptr;
	ComPtr<ID3D12Resource> indexBuff = nullptr;
private:
	DirectX12 *dx12 = DirectX12::Get();

};