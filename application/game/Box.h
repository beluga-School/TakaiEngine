#pragma once
#include "Collision.h"
#include "Tag.h"
#include <list>

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

class Entity;

//当たり判定用に、ID識別ができるようにしたCube
struct IDdCube : public Cube
{
	IDdCube()
	{
		//IDのCountを増やす
		IDdCube::sMasterIDCount++;

		//Entityが産まれたら、そいつのみに対応するIDを割り振る
		//ステージ移動の際にIDを初期化する予定なので、IDが重複する可能性がある
		masterID = IDdCube::sMasterIDCount;
	}

	//自身のIDを取得する関数
	uint32_t GetID()const;

	/// <summary>
	/// 付与IDを0に戻す関数
	/// 全オブジェクト消去の際のみに使用し、それ以外では使用しないこと
	/// </summary>
	static void ResetID();

	Entity* parentEntity = nullptr;

private:
	//全てのオブジェクトが、自身だけのマイナンバーを持つ
	uint32_t masterID = 0;

	//次に付与するIDを管理するナンバー
	static uint32_t sMasterIDCount;
};

//四角で判定をとるオブジェクトの共通項だけ取り出したクラス
class Box : public Obj3d
{
public:
	IDdCube cubecol;

	//自身の当たり判定を作成(更新)
	void CreateCol(const Vector3& pos, const Vector3& scale);

	//自身の描画オブジェクトを更新
	void ColDrawerUpdate(const Vector3& pos, const Vector3& scale);

	Box()
	{
		
	}
};