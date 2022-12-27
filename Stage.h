#pragma once
#include "Block.h"
#include "Mob.h"
#include <list>

class Stage
{
public:
	std::list<Block> blockList;

	void SetStage1();
	void SetBlock(Vector3 pos,Vector3 scale);
	void Update();
	void Draw();

	/// <summary>
	/// ブロックと当たっているかを確認する
	/// プレイヤーの当たり判定を書き換える
	/// </summary>
	/// <param name="player"></param>
	void CheckMobCol(Mob &mob);
};