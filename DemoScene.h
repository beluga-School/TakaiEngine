#pragma once
#include "IScene.h"
#include "Obj.h"
#include "DebugCamera.h"

class DemoScene : public IScene
{
public:
	void LoadResource()override;
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void End()override;

	Obj3d demoline;
	DebugCamera mDebugCamera;
};

