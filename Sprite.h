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
	//頂点バッファ
	ID3D12Resource* vertBuff;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//定数バッファ
	ConstBuffer<ConstBufferDataMaterial> constBufferM;
	ConstBuffer<ConstBufferDataTransform> constBufferT;
};

//スプライトの生成
Sprite SpriteCreate(DirectX12 dx12);

//スプライト共通グラフィックコマンドのセット
void SpriteCommonBeginDraw(DirectX12 dx12,const PipelineSet& pipelineSet,Texture &tex);

void SpriteDraw(const Sprite& sprite, DirectX12 dx12, Texture& tex);