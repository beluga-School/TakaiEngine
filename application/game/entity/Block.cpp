#include "Block.h"
#include "Player.h"
#include "DirectXInit.h"

void Block::Update()
{
	Obj3d::Update(*Camera::sCamera);
}

void Block::Draw()
{
	//b4にプレイヤー位置を送る
	Obj3d::Draw();
}

void Block::DrawMaterial()
{	
	Obj3d::DrawMaterial();
}
