#pragma once
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")
#include <DirectXTex.h>

#include "DirectXInit.h"
#include <vector>
#include "Texture.h"
#include "ConstBuffer.h"

struct VertexPos
{
	XMFLOAT3 pos;
};

struct GeometryConstBufferData
{
	XMMATRIX mat;
};

const int vertexCount = 1;

class GeometryObject
{
public:
	ConstBuffer<GeometryConstBufferData> constBufferGeometry;

	//VertexPos vertices[vertexCount];

	D3D12_RESOURCE_DESC resDesc{};

	//頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	VertexPos verticesPoint[vertexCount];

	void CreateModel();
	
	void Update(XMMATRIX& matView, XMMATRIX& matProjection);

	void Draw(Texture* texture);
};
