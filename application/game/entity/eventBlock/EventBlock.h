#pragma once
#include "Obj.h"
#include "Block.h"

/*! EventBlock
	土管や大砲など、他のものと当たり判定をしたいオブジェクトが継承するクラスステージ間を移動できる土管クラス
*/
class EventBlock : public Block
{
public:
	EventBlock() :Block()
	{

	};

	virtual void Initialize() = 0;

	virtual void Update() = 0;
	
	virtual void Draw() = 0;

	virtual void HitEffect() = 0;

	virtual ~EventBlock(){};

	std::string trigerName = "";

	bool hit = false;
};
