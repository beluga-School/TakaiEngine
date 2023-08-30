#pragma once
#include "EventBlock.h"
#include "TEasing.h"
#include "Mob.h"

class MoveBlock : public EventBlock
{
public:
	MoveBlock() : EventBlock()
	{
		SetTag(TagTable::MoveBlock);
	};

	void Initialize()override;
	void Update()override;
	void Draw()override;

	void HitEffect()override;

	/// <summary>
	/// 当たっている時に読まれる関数
	/// </summary>
	/// <param name="mob"></param>
	void OnCollide(Mob* mob);

	/// <summary>
	/// 当たっていない時に読まれる関数
	/// </summary>
	/// <param name="mob"></param>
	void NotCollide(Mob* mob);

	Vector3 startpos{};
	Vector3 endpos{};

	std::string id = "";

private:
	TEasing::easeTimer moveTimer = 2.0f;
	//1フレームで移動した量
	Vector3 moveValue{};
	Vector3 oldPos{};
};

