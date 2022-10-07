#pragma once
#include "DirectXInit.h"
#include "Pipeline.h"

void CreateDepthView(DirectX12 &dx12);

void ClearDrawScreen(DirectX12 &dx12);

void PreDraw(DirectX12& dx12,PipelineSet objectPipelineSet);

void PostDraw(DirectX12 &dx12);