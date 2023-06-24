#pragma once
#include <IScene.h>
#include <Pipeline.h>
#include <ViewProjection.h>
#include <Obj.h>
#include <Input.h>
#include <DebugCamera.h>
#include "Obj3dFBX.h"

class FBXLoadDemoScene : public IScene
{
public:
	void Initialize();
	void Update();
	void Draw();
	void End();

private:

	Camera* mCamera = Camera::sCamera;

	Obj3d mSphere;
	Obj3d mSkydome;

	DebugCamera mDebugCamera;

	Obj3dFBX mSphereFBX;
};

