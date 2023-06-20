#pragma once
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include "DirectXInit.h"

enum BLEND_MODE
{
	ADD,
	SUB,
	INV,
	ALPHA,
};

struct ShaderName
{
	std::string shaderName = "";
	enum class ShaderType
	{
		VS,
		PS,
		GS,
		ER,
	}shaderType = ShaderType::ER;
};

struct PipelineSet
{
	//�p�C�v���C���X�e�[�g
	ComPtr<ID3D12PipelineState> mPipelinestate;
	//���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> mRootsignature;

	//�ݒ�p�ϐ��Q(���̕ϐ������O�ɏ��������Ă���Create()���Ăяo��)
	
	//�V�F�[�_�[��
	ShaderName vs = { "",ShaderName::ShaderType::VS };
	ShaderName ps = { "",ShaderName::ShaderType::PS };
	ShaderName gs = { "",ShaderName::ShaderType::GS };

	//�O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	//�ݒ�����Ƀp�C�v���C���𐶐�
	PipelineSet Create();
};

PipelineSet CreateObject3DPipeline();

PipelineSet CreateLambertPipeline();

PipelineSet CreateSpritePipeline();

PipelineSet CreateGeometryPipeline();