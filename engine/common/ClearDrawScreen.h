#pragma once
#include "Pipeline.h"

void CreateDepthView();

void ClearDrawScreen();

void BasicObjectPreDraw(PipelineSet objectPipelineSet);

void GeometryObjectPreDraw(PipelineSet geometryPipelineSet);

void PostDraw();