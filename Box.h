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

//四角で判定をとるオブジェクトの共通項だけ取り出したクラス
class Box : public Obj3d
{
public:
	Cube cubecol;

	//自身の当たり判定を作成(更新)
	void CreateCol();
	void CreateCol(const Vector3& pos, const Vector3& scale);

	//産まれた時点でマネージャーに登録
	Box()
	{
		//Register();
	}
};