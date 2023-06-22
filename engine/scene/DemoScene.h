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
	
	Camera* mCamera = Camera::sCamera;

	Obj3d mSkydome;
	
	Sprite mSlime;

	std::list<Obj3d> mObj3ds;

	GUI mGui = std::string("postion");

	DebugCamera mDebugCamera;

	Obj3d testplayer;

	std::string currentLevel = "";
};

