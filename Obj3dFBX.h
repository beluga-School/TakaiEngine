#pragma once
#include "Obj.h"

class Obj3dFBX
{
public:
	void Update();
	void Draw();
	void DrawMaterial();
	void SetModel(const Model* model);

	std::vector<Obj3d> mObj3ds;

	int32_t mMeshesSize;
};

