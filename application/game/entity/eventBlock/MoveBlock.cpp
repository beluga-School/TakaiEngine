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

	moveValue = { 0,0,0 };

	oldPos = position;

	//座標を更新
	position = TEasing::lerp(startpos, endpos, moveTimer.GetTimeRate());

	moveValue = position - oldPos;

	box.CreateCol(position, box.scale);
	box.ColDrawerUpdate(position, box.scale);

	Obj3d::Update(*Camera::sCamera);

	//移動量を足す、はなんかやり方を工夫しないとダメそう
	//座標をmobの当たり判定より先に固定して、後からモブの移動を加算する方式なら？
}

void MoveBlock::Draw()
{
	Obj3d::DrawMaterial();
}

void MoveBlock::HitEffect()
{
}

void MoveBlock::OnCollide(Mob* mob)
{
	//ムーブブロックで加算される値をmobに作り、そこに値を代入
	//計算するときに値があるなら計算する感じに
	mob->moveBlockHit = true;
	
	//移動ブロック用に座標を記録する
	mob->moveBlockPosition = moveValue;
}
