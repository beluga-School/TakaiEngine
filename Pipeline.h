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
	//パイプラインステート
	ComPtr<ID3D12PipelineState> pipelinestate;
	//ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootsignature;
};

PipelineSet CreateObject3DPipeline();

PipelineSet CreateSpritePipeline();

PipelineSet CreateGeometryPipeline();