#pragma once
#include <IScene.h>
#include <Pipeline.h>
#include <ViewProjection.h>
#include <Obj.h>
#include <Input.h>
#include <DebugCamera.h>

class Obj3dFBX
{
public:

	void Update();
	void Draw();
	void DrawMaterial();
	void SetModel(const Model* model);

	std::vector<Obj3d> mSpherefbx;

	int32_t mMeshesSize;
};

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

