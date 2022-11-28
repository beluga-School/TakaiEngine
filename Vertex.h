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

//���_�f�[�^�\����
struct Vertex
{
	XMFLOAT3 pos;	//xyz���W
	XMFLOAT3 normal;	//�@���x�N�g��
	XMFLOAT2 uv;	//uv���W
};

class VertexData
{
public:
	//UINT sizeVB;
	D3D12_RESOURCE_DESC resDesc{};
	D3D12_INDEX_BUFFER_VIEW ibView{};
	//���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	void CreateVertex(std::vector<Vertex> vertices, std::vector<uint16_t> indices);
private:
	DirectX12 *dx12 = DirectX12::GetInstance();

	ComPtr<ID3D12Resource> vertBuff = nullptr;
	ComPtr<ID3D12Resource> indexBuff = nullptr;
};