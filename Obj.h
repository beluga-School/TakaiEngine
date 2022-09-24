#pragma once
#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")
#include "DirectXInit.h"
#include "Model.h"
#include "ConstBuffer.h"
#include "Texture.h"
#include <memory>

class Obj3d
{
public:
	ConstBuffer<ConstBufferDataTransform> constBufferT;
	ConstBuffer<ConstBufferDataMaterial> constBufferM;

	XMFLOAT3 scale = { 1,1,1 };
	XMFLOAT3 rotation = { 0,0,0 };
	XMFLOAT3 position = { 0,0,0 };

	XMMATRIX matWorld;	//���[���h�ϊ��s��
	//XMMATRIX matScale;	//�X�P�[�����O�s��
	//XMMATRIX matRot;	//��]�s��
	//XMMATRIX matTrans;	//���s�ړ��s��

	std::unique_ptr<Obj3d> parent;

	Model* model = nullptr;
public:
	void Initialize();
	void SetModel(Model *model);
	void Update(XMMATRIX& matView, XMMATRIX& matProjection);
	void Draw(ID3D12GraphicsCommandList* commandList, Texture& texture);
};

