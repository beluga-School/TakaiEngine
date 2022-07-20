#pragma once
#include "DirectXInit.h"

struct PipelineSet
{
	//パイプラインステートの生成
	ComPtr<ID3D12PipelineState> pipelineState;
	//ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootSignature;
};

PipelineSet ObjectPipelineCreate3D(DirectX12 DX12);

PipelineSet ObjectPipelineCreate2D(DirectX12 DX12);