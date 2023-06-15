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

struct PipelineSet
{
	//�p�C�v���C���X�e�[�g
	ComPtr<ID3D12PipelineState> mPipelinestate;
	//���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> mRootsignature;
};

PipelineSet CreateObject3DPipeline();

PipelineSet CreateLambertPipeline();

PipelineSet CreateSpritePipeline();

PipelineSet CreateGeometryPipeline();