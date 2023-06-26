#pragma once
#include "IScene.h"
#include "DebugCamera.h"

class MultiRenderScene :
    public IScene
{
public:
	void Initialize();
	void Update();
	void Draw();
	void End();

private:
	DebugCamera mDebugCamera;

	Obj3d mSkydome;

	Obj3d cube;

	std::string pipeline = "MultiRender";
};

