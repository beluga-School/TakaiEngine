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
	//パイプラインステート
	ComPtr<ID3D12PipelineState> mPipelinestate;
	//ルートシグネチャ
	ComPtr<ID3D12RootSignature> mRootsignature;

	//設定用変数群(この変数を事前に書き換えてからCreate()を呼び出す)
	
	//シェーダー名
	ShaderName vs = { "",ShaderName::ShaderType::VS };
	ShaderName ps = { "",ShaderName::ShaderType::PS };
	ShaderName gs = { "",ShaderName::ShaderType::GS };

	//グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	//設定を元にパイプラインを生成
	PipelineSet Create();
};

PipelineSet CreateObject3DPipeline();

PipelineSet CreateLambertPipeline();

PipelineSet CreateSpritePipeline();

PipelineSet CreateGeometryPipeline();