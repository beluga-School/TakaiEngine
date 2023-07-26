#pragma once
#include "Obj.h"
#include "Box.h"
#include <vector>

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
};

//3d描画される全てのオブジェクト
class Entity : public Obj3d
{
public:
	//判定を取るときに使うタグ
	std::vector<TagTable> taglist;

	bool IsDead() {
		return isDead;
	}

	bool CheckTag(TagTable check);

	void Register();

	//Entityは必ず四角の当たり判定を持つ
	Box box;

	Entity()
	{
		//Register();
		taglist.push_back(TagTable::Box);
	}

protected:
	bool isDead = false;
};