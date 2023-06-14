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
	PipelineSet object3dPipelineSet;
	Camera* camera = Camera::mCamera;

	Obj3d sphere;
	Obj3d skydome;
	Input::Keyboard* input = Input::Keyboard::Get();

	//const wchar_t* modelFile = L"Resources/Cube_gltf/cube.glb";
	//const wchar_t* modelFile = L"Resources/Cube_two/Cube_two.glb";
	const wchar_t* modelFile = L"Resources/sphere_fbx/sphere.fbx";
	//const wchar_t* modelFile = L"Resources/Alicia/Alicia_solid_MMD.fbx";
	
	DebugCamera debugCamera;

	std::vector<Obj3d> spherefbx;

	std::vector<Mesh> meshes;
	std::vector<VertexData> vertexDatas;
};

