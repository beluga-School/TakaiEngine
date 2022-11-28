#pragma once
#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")
#include "DirectXInit.h"
#include "Model.h"
#include "Texture.h"
#include <memory>

struct ConstBufferDataB1
{
	Vector3 ambient;	//�A���r�G���g�W��
	float pad1;			//�p�f�B���O
	Vector3 diffuse;	//�f�B�t���[�Y�W��
	float pad2;			//�p�f�B���O
	Vector3 specular;	//�X�y�L�����[�W��
	float alpha;
};

struct ConstBufferBrightness
{
	XMFLOAT4 brightness;		//�J���[
};

class Obj3d
{
public:
	ConstBuffer<ConstBufferDataTransform> constBufferT;
	ConstBuffer<ConstBufferDataMaterial> constBufferM;
	ConstBuffer<ConstBufferBrightness> constBufferB;

	ConstBuffer<ConstBufferDataB1> constBufferMaterial;

	XMFLOAT3 scale = { 1.0f,1.0f,1.0f };
	XMFLOAT3 rotation = { 0,0,0 };
	XMFLOAT3 position = { 0,0,0 };

	XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f };

	XMMATRIX matWorld;	//���[���h�ϊ��s��
	//XMMATRIX matScale;	//�X�P�[�����O�s��
	//XMMATRIX matRot;	//��]�s��
	//XMMATRIX matTrans;	//���s�ړ��s��

	bool notScaleFlag = false;

	Obj3d* parent = nullptr;
	Texture *texture = nullptr;

	Model* model = nullptr;
public:
	void Initialize();
	void SetModel(Model *model);
	void SetTexture(Texture *texture);
	void Update(XMMATRIX& matView, XMMATRIX& matProjection);

	Vector3 GetWorldTrans();

	void Draw();
	void DrawMaterial();
};

