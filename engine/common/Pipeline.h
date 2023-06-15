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
	ComPtr<ID3D12PipelineState> mPipelinestate;
	//ルートシグネチャ
	ComPtr<ID3D12RootSignature> mRootsignature;
};

PipelineSet CreateObject3DPipeline();

PipelineSet CreateLambertPipeline();

PipelineSet CreateSpritePipeline();

PipelineSet CreateGeometryPipeline();