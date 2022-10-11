#pragma once
#include "DirectXInit.h"
#include "Pipeline.h"

void CreateDepthView();

void ClearDrawScreen();

void PreDraw(PipelineSet objectPipelineSet);

void PostDraw();