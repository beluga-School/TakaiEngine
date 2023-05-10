#pragma once
#include "Pipeline.h"
#include "ViewProjection.h"
#include "Obj.h"

class DemoScene
{
public:
	void Initialize();
	void Update();
	void Draw();
	void End();

private:
	PipelineSet object3dPipelineSet;
	Camera* camera = Camera::camera;

	Obj3d sphere;
};

