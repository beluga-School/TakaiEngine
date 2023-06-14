#pragma once
#include "Obj.h"
#include "Collision.h"

enum class CheckDirection
{
	CD_UP,
	CD_DOWN,
	CD_CENTER,
	CD_BACK,
	CD_LEFT,
	CD_RIGHT,
};

class Block : public Obj3d
{
public:

	Cube cubeCol;

	Vector3 upVec;
	Vector3 centerVec;
	Vector3 leftVec;

	Block() {
		Initialize();
	};
	
	void Initialize();
	void Update();

	/// <summary>
	/// ���肵�������̂��A���̃u���b�N���猩�Ăǂ��瑤�ɂ��邩�̃t���O���v�Z����֐�
	/// �g������Mob��CheckHitColStage���Q��
	/// </summary>
	/// <param name="cubeCol"></param>
	/// <param name="CD"></param>
	/// <returns></returns>
	bool CheckDirections(const Cube& cubeCol, const CheckDirection& CD)const;

	void ExtractVec();
};