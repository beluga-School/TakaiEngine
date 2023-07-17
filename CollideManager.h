#pragma once
#include "Collision.h"
#include <list>
#include "Obj.h"

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

//ブロックの方向を確認するためのenum
enum class CheckDirection
{
	CD_UP,
	CD_DOWN,
	CD_CENTER,
	CD_BACK,
	CD_LEFT,
	CD_RIGHT,
};

//当たり判定を持つなら、他の当たり判定オブジェクト全てと判定をとる
//->判定取るよリストに入れる
//こいつは生き物なので、全部の判定と取るよっていうタグ(string)で判別できるようにする
//こいつは地面とかなので、生き物としか判定を取らないよっていうタグで判別

enum class TagTable
{
	None = -1,
	Mob = 0,
	Block = 1,
};

//四角で判定をとるオブジェクトの共通項だけ取り出したクラス
class Box : public Obj3d
{
public:
	Cube cubecol;

	//判定を取るときに使うタグ
	TagTable tag = TagTable::None;

	//自身の当たり判定を作成(更新)
	void CreateCol();
	void CreateCol(const Vector3& pos,const Vector3 &scale);

	//マネージャーに自身を登録
	void Register();
	
	//産まれた時点でマネージャーに登録
	Box()
	{
		Register();
	}
};

class Block : public Box
{
public:
	Block() {
		tag = TagTable::Block;
	};
};

class Mob : public Box
{
public:
	Vector3 moveValue = { 0,0,0 };

	std::list<Cube> hitListY;

	Mob() {
		tag = TagTable::Mob;
	};
};

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