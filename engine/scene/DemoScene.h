#pragma once
#include "Pipeline.h"
#include "Camera.h"
#include "Obj.h"
#include "IScene.h"
#include "Input.h"
#include <LevelLoader.h>
#include <Sprite.h>
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

	DebugCamera mDebugCamera;

	Obj3d testplayer;

	std::string currentLevel = "";
	std::string oldcurrentLevel = "";

	std::string pipeline = "Phong";

	bool showObject = true;
	bool showCollider = false;
	bool showSpawn = true;
	bool showEvent = true;

	bool showSkydome = true;
};

