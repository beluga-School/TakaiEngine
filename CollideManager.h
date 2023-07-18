#pragma once
#include "Collision.h"
#include <list>
#include "Obj.h"
#include "Box.h"
#include "Mob.h"
#include "Block.h"

//リストの中に同じオブジェクトがあれば、そのオブジェクトを入れないプッシュバック
template <class T>
void UniqueObjectPushBack(std::list<T>& list, const T& col)
{
	for (auto itr = list.begin(); itr != list.end(); itr++)
	{
		//同じ要素が見つかったら止める
		if (*itr == col)
		{
			return;
		}
		//回しきれたら同じ要素がない
	}
	//から入れる
	list.push_back(col);
}

//リストの中に同じオブジェクトがあれば、そのオブジェクトを削除する
template <class T>
void UniqueObjectErase(std::list<T>& list, const T& col)
{
	for (auto itr = list.begin(); itr != list.end(); itr++)
	{
		//同じ要素が見つかったら
		if (*itr == col)
		{
			//消す
			list.erase(itr);
			return;
		}
	}
}

class CollideManager
{
public:
	std::list<Box*> allCols;
	void Update();

	static CollideManager* Get()
	{
		static CollideManager instance;
		return &instance;
	}

	//手動で当たり判定を読んでおきたいとき用に、別で分ける
	void CheckCollide(Box* check,Box* collide);

private:
	CollideManager(){};
	~CollideManager(){};

	//押し戻し処理
	void Osimodosi(Mob& check, const Block& collide);

	//checkとcollideの位置関係を見て、右にいるか左にいるかなどでtrue/falseを返す
	bool CheckDirections(const Cube& check, const Cube& collide, const CheckDirection& CD);
};