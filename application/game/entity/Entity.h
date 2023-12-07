#pragma once
#include "Obj.h"
#include "Box.h"
#include "Tag.h"
#include <vector>

/*! Entity
	ゲーム内で3d描画される全てのオブジェクトが継承するクラス
*/
class Entity : public Obj3d
{
public:
	Entity()
	{
		SetTag(TagTable::Box);

		box.cubecol.parentEntity = this;
	}

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	virtual ~Entity() {};

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

	/// <summary>
	/// 当たり判定をマネージャーに登録する
	/// </summary>
	void Register();

	/// <summary>
	/// 当たり判定に付与された、オブジェクトごとに一意に定義されているIDを返す
	/// </summary>
	/// <returns></returns>
	uint32_t GetID()const;

	bool IsDead() const {
		return isDead;
	}

	//Entityは必ず四角の当たり判定を持つ
	Box box;

	bool isTexDraw = false;

	//イベント名を保持
	std::string eventName_ = "";

	//アクティブ(更新がされる状態)かどうか
	bool mActive = true;

protected:
	bool isDead = false;

private:
	//判定を取るときに使うタグ
	std::vector<TagTable> taglist;
};