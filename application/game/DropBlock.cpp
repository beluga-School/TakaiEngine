#include "DropBlock.h"

void DropBlock::LoadResource()
{
}

void DropBlock::Initialize()
{
	Obj3d::Initialize();
}

void DropBlock::Update()
{
	CollsionUpdate();

	Obj3d::Update(*Camera::sCamera);
}

void DropBlock::Draw()
{
	if (isTexDraw)
	{
		Obj3d::Draw();
	}
	else
	{
		Obj3d::DrawMaterial();
	}
}

void DropBlock::HitEffect()
{
	
}

void DropBlock::Encount()
{
}
