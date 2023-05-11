#pragma once
#include "Pipeline.h"
#include "ViewProjection.h"
#include "Obj.h"
#include "IScene.h"
#include "Input.h"

class DemoScene : public IScene
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
	Input* input = Input::GetInstance();
};

