#pragma once
#include <vector>
#include "Vertex.h"
#include "ConstBuffer.h"
#include "Shader.h"
#include "Texture.h"

struct PipelineSet
{
	ComPtr<ID3D12PipelineState> pipelineState;

	ComPtr<ID3D12RootSignature> rootSignature;
};

class Sprite
{
private:
	//std::vector<VertexPosUv> vertices;	//こいつらが頑張ればmodel読み込みもいけるっピ
	//SpriteKozotai kozotai;
public:
	//XMFLOAT3 scale = { 1,1,1 };
	//XMFLOAT3 rotation = { 0,0,0 };
	//XMFLOAT3 position = { 0,0,0 };

	//XMMATRIX matWorld;	//ワールド変換行列

	//パイプラインを作る関数(多分別のクラスにした方が使いまわせる？)
	PipelineSet PipelineCreate(DirectX12 DX12);
	//void CreateSprite(DirectX12 dx12_);

};

struct SpriteKozotai
{

	ID3D12Resource* vertBuff;
	VertexDataS vertS;
	ConstBuffer<ConstBufferDataTransform> constBufferT;

	static SpriteKozotai SpriteCreate(DirectX12 DX12);

	void PreDraw(ID3D12GraphicsCommandList* commandList, const PipelineSet& pipelineSet, Texture& texture);
	void Draw(const Sprite& sprite, ID3D12GraphicsCommandList* commandList);
};
