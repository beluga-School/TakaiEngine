#pragma once
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include "DirectXInit.h"
#include <map>

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
	ComPtr<ID3D12PipelineState> mPipelinestate{};
	//���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> mRootsignature{};

	//--�ݒ�p�ϐ��Q(���̕ϐ������O�ɏ��������Ă���Create()���Ăяo��)
	//�V�F�[�_�[��
	ShaderName vs = { "",ShaderName::ShaderType::VS };
	ShaderName ps = { "",ShaderName::ShaderType::PS };
	ShaderName gs = { "",ShaderName::ShaderType::GS };

	//�O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	//�u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc;

	//�C���v�b�g���C�A�E�g
	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;

	//�f�X�N���v�^�����W
	D3D12_DESCRIPTOR_RANGE descriptorRange{};

	//���[�g�p�����[�^
	std::vector<D3D12_ROOT_PARAMETER> rootParams;
	int32_t paramSize = 2;

	//�T���v���[�ݒ�
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};

	//���[�g�V�O�l�`���ݒ�
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};

	int32_t blendMode = ALPHA;

	//�������ꂽ��f�t�H�̐ݒ肪����悤��
	PipelineSet();

	//�ݒ�����Ƀp�C�v���C���𐶐�
	void Create();
};

class PipelineManager
{
public:
	//������Ńp�C�v���C�����擾
	static PipelineSet GetPipeLine(const std::string& name);

	static PipelineManager* Get() {
		static PipelineManager instance;
		return &instance;
	};

	//�p�C�v���C�����ɐ������Ă���
	void Initialize();

private:
	static std::map<std::string, PipelineSet> sPipelines;

	//3D�I�u�W�F�N�g�p�p�C�v���C��
	void Object3DPipeLine();

	void PostEffectPipeLine();

	PipelineManager(){};
	~PipelineManager(){};
};

PipelineSet CreateLambertPipeline();

PipelineSet CreateSpritePipeline();

PipelineSet CreateGeometryPipeline();