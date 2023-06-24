#include "Obj3dFBX.h"


void Obj3dFBX::Update()
{
	for (int32_t i = 0; i < mMeshesSize; i++)
	{
		mObj3ds[i].Update(*Camera::sCamera);
	}
}

void Obj3dFBX::Draw()
{
	for (int32_t i = 0; i < mMeshesSize; i++)
	{
		mObj3ds[i].Draw();
	}
}

void Obj3dFBX::DrawMaterial()
{
	for (int32_t i = 0; i < mMeshesSize; i++)
	{
		mObj3ds[i].DrawMaterial();
	}
}

void Obj3dFBX::SetModel(const Model* model)
{
	mMeshesSize = (int32_t)model->mMeshes.size();

	for (int32_t i = 0; i < mMeshesSize; i++)
	{
		mObj3ds.emplace_back();
		mObj3ds.back().Initialize();
		mObj3ds.back().SetTexture(TextureManager::GetTexture("white"));

		mObj3ds.back().SetModel(model);

	}
}
