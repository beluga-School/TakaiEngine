#pragma once
#define NOMINMAX
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>

#include "DirectXInit.h"
#include <vector>

//���_�f�[�^�\����
struct Vertex
{
	DirectX::XMFLOAT3 pos;	//xyz���W
	DirectX::XMFLOAT3 normal;	//�@���x�N�g��
	DirectX::XMFLOAT2 uv;	//uv���W
};

class VertexData
{
public:
	//UINT sizeVB;
	D3D12_RESOURCE_DESC mResDesc{};
	D3D12_INDEX_BUFFER_VIEW mIbView{};
	//���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW mVbView{};

	void CreateVertex(const std::vector<Vertex> &vertices,const std::vector<uint16_t> &indices);
	
	Microsoft::WRL::ComPtr<ID3D12Resource> mVertBuff = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> mIndexBuff = nullptr;
};