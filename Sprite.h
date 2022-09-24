#pragma once
#include "Vector3.h"
#include "Vector2.h"

#include "DirectXInit.h"
#include <vector>
#include "ConstBuffer.h"

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

#include "Pipeline.h"
#include "Texture.h"

struct VertexPosUV
{
	XMFLOAT3 pos;
	XMFLOAT2 uv;
};

struct Sprite
{
	//���_�o�b�t�@
	ID3D12Resource* vertBuff;
	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//�萔�o�b�t�@
	ConstBuffer<ConstBufferDataMaterial> constBufferM;
	ConstBuffer<ConstBufferDataTransform> constBufferT;
};

//�X�v���C�g�̐���
Sprite SpriteCreate(DirectX12 dx12);

//�X�v���C�g���ʃO���t�B�b�N�R�}���h�̃Z�b�g
void SpriteCommonBeginDraw(DirectX12 dx12,const PipelineSet& pipelineSet,Texture &tex);

void SpriteDraw(const Sprite& sprite, DirectX12 dx12, Texture& tex);