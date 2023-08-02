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
	Player = 5,
	Enemy = 6,
	Star = 7,
	WarpBlock = 8,
	Coin = 9,
	Dokan = 10,
	NoDraw = 11,
};

//3d描画される全てのオブジェクト
class Entity : public Obj3d
{
public:
	bool IsDead() const {
		return isDead;
	}

	/// <summary>
	/// 指定したタグがあるか確認する
	/// </summary>
	/// <param name="check">確認したいタグ</param>
	/// <returns>あったらtrue ないならfalse</returns>
	bool CheckTag(TagTable check);

	/// <summary>
	/// 指定したタグを付ける(重複していた場合は付けない)
	/// </summary>
	/// <param name="check">付けたいタグ</param>
	/// <returns>タグのセットに成功でtrue 重複があったらfalse</returns>
	bool SetTag(TagTable check);
	
	/// <summary>
	/// 指定したタグを削除する
	/// </summary>
	/// <param name="check">削除したいタグ</param>
	/// <returns>タグの削除に成功でtrue タグが見つからない場合false</returns>
	bool DeleteTag(TagTable check);

	void Register();

	//Entityは必ず四角の当たり判定を持つ
	Box box;

	Entity()
	{
		SetTag(TagTable::Box);
	}

protected:
	bool isDead = false;

private:
	//判定を取るときに使うタグ
	std::vector<TagTable> taglist;
};