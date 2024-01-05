#include "MoveBlock.h"

void MoveBlock::Initialize()
{
	Obj3d::Initialize();
	SetTexture(TextureManager::GetTexture("white"));
}

void MoveBlock::Update()
{
	moveTimer.Update();
	invideTimer.Update();

	//乗ったら動き出す
	if (isStatic)
	{
		if (moveTimer.GetEnd())
		{
			if (!invideTimer.GetStarted())invideTimer.Start();
		}
		if (invideTimer.GetEnd())
		{
			invideTimer.Reset();
			moveTimer.ReverseStart();
		}
		//プレイヤーが触ったフラグを解除
		if (moveTimer.GetReverseEnd())
		{
			checkP = false;
		}
	}
	//常に動いている
	else
	{
		if (moveTimer.GetEnd())
		{
			moveTimer.ReverseStart();
		}
		if (moveTimer.GetReverseEnd())
		{
			moveTimer.Start();
		}
	}

	moveValue = { 0,0,0 };

	oldPos = position;

	//座標を更新
	position = TEasing::lerp(startpos, endpos, moveTimer.GetTimeRate());

	moveValue = position - oldPos;

	box.CreateCol(position, box.scale, rotation);
	box.ColDrawerUpdate(position, box.scale);

	Obj3d::Update(*Camera::sCamera);

	//移動量を足す、はなんかやり方を工夫しないとダメそう
	//座標をmobの当たり判定より先に固定して、後からモブの移動を加算する方式なら？
}

void MoveBlock::Draw()
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

	if (!checkP)
	{
		moveTimer.Start();
	}

	checkP = true;
}

void MoveBlock::SetStatic()
{
	isStatic = true;
}

void MoveBlock::TimerStart()
{
	if(!isStatic)moveTimer.Start();
}
