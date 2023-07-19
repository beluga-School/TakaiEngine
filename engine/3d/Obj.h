#pragma once
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include "DirectXInit.h"
#include "Model.h"
#include "Texture.h"
#include <memory>
#include "Matrix4.h"
#include "LightGroup.h"
#include "ViewProjection.h"
#include "Pipeline.h"
#include "TEasing.h"

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
	Float4 brightness;		//�J���[
};

struct ConstBufferDataTransform {
	//Matrix4 mat;	//3D�ϊ��s��
	Matrix4 viewproj;	//�r���[�v���s��
	Matrix4 world;		//���[���h�s��
	Vector3 cameraPos;	//�J�������W(���[���h�s��)
	float pad1;
	Vector3 cameraDir;  //�J�����̎����x�N�g�� <-������������Ȃ����H�H�H�H
};

struct ConstBufferDataOutLine {
	Float4 color;
	float thickness;
};

class Obj3d
{
public:

	ConstBuffer<ConstBufferDataTransform> constBufferT;
	ConstBuffer<ConstBufferBrightness> constBufferB;

	ConstBuffer<ConstBufferDataB1> constBufferB1;
	ConstBuffer<ConstBufferDataOutLine> constBufferOutLine;

	Vector3 scale = { 1.0f,1.0f,1.0f };
	Vector3 rotation = { 0,0,0 };
	Vector3 position = { 0,0,0 };

	Float4 color_ = { 1.0f,1.0f,1.0f,1.0f };

	Matrix4 matWorld;

	bool notScaleFlag = false;

	Obj3d* parent = nullptr;
	const Texture *TEXTURE = nullptr;

	const Model* MODEL = nullptr;

	bool mIsVisiable = true;

public:
	void Initialize();
	void SetModel(const Model *model);
	void SetTexture(const Texture *texture);
	void Update(const Camera& camera);

	Vector3 GetWorldTrans();

	void Draw();
	void DrawMaterial();

	void DrawOutLine();

	void DrawBlendTexture(const Texture& subTex,const Texture& maskTex);

	void SetOutLineState(const Float4& color,float thickness);
	void SetOutLineAlpha(const float& alpha);

	TEasing::easeTimer transparentTimer = 0.5f;
protected:
	//�A�E�g���C������̐ݒ�
	Float4 mOutLineColor = {0,0,0,1.0f};
	float mOutLineThickness = 0.0f;
};