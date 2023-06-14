#pragma once
#include "Pipeline.h"
#include "ViewProjection.h"
#include "Obj.h"
#include "IScene.h"
#include "Input.h"
#include <LevelLoader.h>
#include <Sprite.h>
#include <ImguiManager.h>
#include <DebugCamera.h>

class DemoScene : public IScene
{
public:
	void Initialize();
	void Update();
	void Draw();
	void End();

private:
	void SetObject(LevelData& data);
	
	PipelineSet object3dPipelineSet;
	Camera* camera = Camera::mCamera;

	Obj3d skydome;
	Input::Keyboard* input = Input::Keyboard::Get();

	Sprite slime;

	std::list<Obj3d> obj3ds;

	GUI gui = std::string("postion");

	DebugCamera cameraObject;
};

