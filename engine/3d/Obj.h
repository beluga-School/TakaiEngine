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
	float pad2;
};

struct ConstBufferDataOutLine {
	Float4 color;
	float thickness;
};

struct ConstBufferDisolve {
	float value;
};

struct ConstBufferCamPlayer {
	Vector3 playerPos;
	float radius;
};

//����`�掞�̒萔
namespace SpecialDraw
{
	typedef int32_t DISOLVE;
	typedef int32_t TEXTUREBLEND;

	static DISOLVE DISOLVE_;
	static TEXTUREBLEND TEXTUREBLEND_;
}

class Obj3d
{
public:

	ConstBuffer<ConstBufferDataTransform> constBufferT;
	ConstBuffer<ConstBufferBrightness> constBufferB;

	ConstBuffer<ConstBufferDataB1> constBufferB1;
	ConstBuffer<ConstBufferDataOutLine> constBufferOutLine;

	ConstBuffer<ConstBufferDisolve> constBufferDisolve;
	ConstBuffer<ConstBufferCamPlayer> constBufferCamPlayer;

	Vector3 scale = { 1.0f,1.0f,1.0f };
	Vector3 rotation = { 0,0,0 };
	Vector3 position = { 0,0,0 };

	Float4 color_ = { 1.0f,1.0f,1.0f,1.0f };

	Matrix4 matWorld;

	Obj3d* parent = nullptr;
	
	const Texture *TEXTURE = nullptr;

	const Model* MODEL = nullptr;

	//�`��t���O
	bool mIsVisiable = true;

	//�f�B�]���u�̐i�s�x(0�`1�ŊǗ�)
	float disolveVal = 0;

public:
	void Initialize();
	void SetModel(const Model *model);
	void SetTexture(const Texture *texture);
	void Update(const Camera& camera);

	Vector3 GetWorldTrans();

	void Draw();
	void DrawMaterial();

	///---����`��
	//�A�E�g���C���Ŏg�p����`��
	void DrawOutLine();

	//�e�N�X�`���u�����h�Ŏg�p����`��
	void DrawSpecial(SpecialDraw::TEXTUREBLEND drawkey,const Texture& subTex,const Texture& maskTex);
	void DrawSpecial(SpecialDraw::DISOLVE drawkey,const Texture& maskTex);
	
	///---������ӂ͊֐����ЂƂɂ��āA�萔�ŐU�镑����ς���悤�Ȑ݌v�ɂ�����
	///�萔�̒��g�ŃI�[�o�[���[�h���ς���銴���ɂ�����

	void SetOutLineState(const Float4& color,float thickness);
	void SetOutLineAlpha(const float& alpha);

	TEasing::easeTimer transparentTimer = 0.5f;
protected:
	//�A�E�g���C������̐ݒ�
	Float4 mOutLineColor = {0,0,0,1.0f};
	float mOutLineThickness = 0.0f;
};