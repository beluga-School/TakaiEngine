#include "MoveBlock.h"

void MoveBlock::Initialize()
{
	Obj3d::Initialize();
	SetTexture(TextureManager::GetTexture("white"));

	moveTimer.Start();
}

void MoveBlock::Update()
{
	moveTimer.Update();

	if (moveTimer.GetEnd())
	{
		moveTimer.ReverseStart();
	}
	if (moveTimer.GetReverseEnd())
	{
		moveTimer.Start();
	}

	//ç¿ïWÇçXêV
	position = TEasing::lerp(startpos, endpos, moveTimer.GetTimeRate());
	box.CreateCol(position, box.scale);
	box.ColDrawerUpdate(position, box.scale);

	Obj3d::Update(*Camera::sCamera);

}

void MoveBlock::Draw()
{
	Obj3d::DrawMaterial();
}

void MoveBlock::HitEffect()
{
}