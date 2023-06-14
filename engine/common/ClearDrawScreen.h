#pragma once
#include "Pipeline.h"

void CreateDepthView();

void ClearDrawScreen();

void BasicObjectPreDraw(const PipelineSet& objectPipelineSet);

void GeometryObjectPreDraw(const PipelineSet& geometryPipelineSet);

void PostDraw();