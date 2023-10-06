#pragma once
#include "Obj.h"
#include "Box.h"
#include "Tag.h"
#include <vector>

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

		box.cubecol.parentEntity = this;
	}

	/// <summary>
	/// 当たり判定に付与された、オブジェクトごとに一意に定義されているIDを返す
	/// </summary>
	/// <returns></returns>
	uint32_t GetID()const;

	bool isTexDraw = false;

protected:
	bool isDead = false;

private:
	//判定を取るときに使うタグ
	std::vector<TagTable> taglist;
};