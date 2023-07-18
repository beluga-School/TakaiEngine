#pragma once
#include "Collision.h"

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
	Cannon = 2,
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
	void CreateCol(const Vector3& pos, const Vector3& scale);

	//マネージャーに自身を登録
	void Register();

	//産まれた時点でマネージャーに登録
	Box()
	{
		Register();
	}
};