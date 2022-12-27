#pragma once
#include "Block.h"
#include "Mob.h"
#include <list>

class Stage
{
public:
	std::list<Block> blockList;

	void SetStage1();
	void SetBlock(Vector3 pos,Vector3 scale);
	void Update();
	void Draw();

	/// <summary>
	/// �u���b�N�Ɠ������Ă��邩���m�F����
	/// �v���C���[�̓����蔻�������������
	/// </summary>
	/// <param name="player"></param>
	void CheckMobCol(Mob &mob);
};