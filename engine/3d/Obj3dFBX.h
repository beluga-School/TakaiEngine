#pragma once
#include "Obj.h"

/*! Obj3dFBX
	FBX描画用3Dオブジェクト
*/
class Obj3dFBX
{
public:
	void Update();
	
	void Draw();
	
	void DrawMaterial();

	//モデルを設定
	void SetModel(Model* model);

	std::vector<Obj3d> mObj3ds;

	int32_t mMeshesSize;
};

