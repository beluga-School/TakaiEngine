#include "Block.h"

void Block::Initialize()
{
	Obj3d::Initialize();
}

void Block::Update()
{
	Obj3d::Update(*Camera::sCamera);
}

void Block::Draw()
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
