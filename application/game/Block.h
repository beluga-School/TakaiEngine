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
	/// 判定したい物体が、そのブロックから見てどちら側にいるかのフラグを計算する関数
	/// 使い方はMobのCheckHitColStageを参照
	/// </summary>
	/// <param name="cubeCol"></param>
	/// <param name="CD"></param>
	/// <returns></returns>
	bool CheckDirections(const Cube& cubeCol, const CheckDirection& CD)const;

	void ExtractVec();
};