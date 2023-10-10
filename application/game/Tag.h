#pragma once

//当たり判定を持つなら、他の当たり判定オブジェクト全てと判定をとる
//->判定取るよリストに入れる
//こいつは生き物なので、全部の判定と取るよっていうタグ(string)で判別できるようにする
//こいつは地面とかなので、生き物としか判定を取らないよっていうタグで判別
enum class TagTable
{
	None = -1,
	Mob = 0,
	Block = 1,
	Cannon = 2,
	Box = 3,
	Collsion = 4,
	Player = 5,
	Enemy = 6,
	Star = 7,
	WarpBlock = 8,
	Coin = 9,
	Dokan = 10,
	NoDraw = 11,
	MoveBlock = 12,
	EventTriggerBox = 13,
};