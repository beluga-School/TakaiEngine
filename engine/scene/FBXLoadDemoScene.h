#pragma once
#include <IScene.h>
#include <Pipeline.h>
#include <ViewProjection.h>
#include <Obj.h>
#include <Input.h>
#include <DebugCamera.h>

class FBXLoadDemoScene : public IScene
{
public:
	void Initialize();
	void Update();
	void Draw();
	void End();

private:
	PipelineSet mObject3dPipelineSet;
	Camera* mCamera = Camera::sCamera;

	Obj3d mSphere;
	Obj3d mSkydome;
	//Input::Keyboard* input = Input::Keyboard::Get();

	//const wchar_t* modelFile = L"Resources/Cube_gltf/cube.glb";
	//const wchar_t* modelFile = L"Resources/Cube_two/Cube_two.glb";
	const wchar_t* mModelFile = L"Resources/sphere_fbx/sphere.fbx";
	//const wchar_t* modelFile = L"Resources/Alicia/Alicia_solid_MMD.fbx";
	
	DebugCamera mDebugCamera;

	std::vector<Obj3d> mSpherefbx;

	std::vector<Mesh> mMeshes;
	std::vector<VertexData> mVertexDatas;
};

