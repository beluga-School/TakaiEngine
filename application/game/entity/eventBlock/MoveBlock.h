#pragma once
#include "Entity.h"
#include "TEasing.h"
#include "Mob.h"

/*! MoveBlock
	2点間を移動するブロックのクラス
*/
class MoveBlock : public Entity
{
public:
	MoveBlock() : Entity()
	{
		SetTag(TagTable::MoveBlock);
	};

	void Initialize()override;
	void Update()override;
	void Draw()override;

	//ヒット時の処理
	void HitEffect();

	/// <summary>
	/// 当たっている時に読まれる関数
	/// </summary>
	/// <param name="mob"></param>
	void OnCollide(Mob* mob);

	//乗ったときに動き出すような挙動になるフラグを立てる
	void SetStatic();

	void TimerStart();

public:
	Vector3 startpos{};
	Vector3 endpos{};

	std::string id = "";

private:
	TEasing::easeTimer moveTimer = 2.0f;
	//1フレームで移動した量
	Vector3 moveValue{};
	Vector3 oldPos{};

	//プレイヤーが乗ったときに動き出すような挙動になるフラグ
	bool isStatic = false;

	bool checkP = false;

	TEasing::easeTimer invideTimer = 2.0f;
};

