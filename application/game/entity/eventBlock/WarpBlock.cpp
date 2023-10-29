#include "WarpBlock.h"
#include "StageChanger.h"

void WarpBlock::Initialize()
{
	SetModel(ModelManager::GetModel("eventtriger"));
	SetTexture(TextureManager::GetTexture("white"));
}

void WarpBlock::Update()
{
	Obj3d::Update(*Camera::sCamera);
}

void WarpBlock::Draw()
{
	Obj3d::DrawMaterial();
}

void WarpBlock::HitEffect()
{
	if (LevelLoader::Get()->GetData(trigerName) != nullptr)
	{
		StageChanger::Get()->ChangeLevel(*LevelLoader::Get()->GetData(trigerName));
	}
}
