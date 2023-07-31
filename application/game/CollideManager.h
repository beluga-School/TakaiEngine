#pragma once
#include "Collision.h"
#include <list>
#include "Obj.h"
#include "Entity.h"
#include "Mob.h"
#include "Block.h"
#include "Enemy.h"
#include "Player.h"

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
	std::list<Entity*> allCols;
	
	//HPの変更や状態の変化など、ステータスを変更する当たり判定の更新
	void StatusUpdate();
	
	//押し戻し処理など、座標が変化する当たり判定の更新
	void CollideUpdate();

	static CollideManager* Get()
	{
		static CollideManager instance;
		return &instance;
	}

	//手動で当たり判定を読んでおきたいとき用に、別で分ける
	
	//地形更新用
	void CheckCollide(Entity* check, Entity* collide);

	//ステータス更新用
	void CheckStatus(Entity* check, Entity* collide);

private:
	CollideManager(){};
	~CollideManager(){};

	//押し戻し処理
	void Osimodosi(Mob& check, const Block& collide);

	//checkとcollideの位置関係を見て、右にいるか左にいるかなどでtrue/falseを返す
	bool CheckDirections(const Cube& check, const Cube& collide, const CheckDirection& CD);

	//プレイヤーとEnemyの当たり判定
	void CheckPlayerToEnemy(Player& player, Enemy& collide);
};