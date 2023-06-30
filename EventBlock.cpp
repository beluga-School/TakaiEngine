#include "EventBlock.h"
#include "Stage.h"

void EventBlock::Initialize()
{
	SetModel(ModelManager::GetModel("eventtriger"));
	SetTexture(TextureManager::GetTexture("white"));
}

void EventBlock::Update()
{
	Obj3d::Update(*Camera::sCamera);
}

void EventBlock::Draw()
{
	Obj3d::DrawMaterial();
}

void EventBlock::HitEffect()
{
	if (LevelLoader::Get()->GetData(trigerName) != nullptr)
	{
		Stage::Get()->ChangeLevel(*LevelLoader::Get()->GetData(trigerName));
	}
}
