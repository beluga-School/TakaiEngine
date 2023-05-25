#pragma once
#include "Pipeline.h"
#include "ViewProjection.h"
#include "Obj.h"
#include "IScene.h"
#include "Input.h"
#include <LevelLoader.h>
#include <Sprite.h>

class DemoScene : public IScene
{
public:
	void Initialize();
	void Update();
	void Draw();
	void End();

private:
	void SetObject(LevelData* data);
	
	PipelineSet object3dPipelineSet;
	Camera* camera = Camera::camera;

	Obj3d cube;
	Obj3d skydome;
	Input* input = Input::Get();

	Sprite slime;

	std::list<Obj3d> obj3ds;
};

