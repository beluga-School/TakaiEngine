#pragma once
#include "Obj.h"
#include "Collision.h"

class Mob : public Obj3d
{
public:
	Mob();

	void Initialize();
	void Update();

	void AddGravity();

	//void CheckHitColStage(Stage& stage);
	
	/// <summary>
	/// 移動方向ベクトルの抽出
	/// </summary>
	void PreMove();

	/// <summary>
	/// ワールド行列の更新と地面判定の初期化と移動値の初期化
	/// </summary>
	void PostMove();

	//受ける重力
	float gravity;

	//ジャンプ力
	float jumpPower;
	
	//移動量(秒単位で動く量)
	float moveMag;

	//地面に設置しているか
	bool onGround = false;

	//1フレームで移動する値
	Vector3 moveValue;

	//正面ベクトル
	Vector3 centerVec;

	//左方向ベクトル
	Vector3 leftVec;

	//四角の当たり判定
	Cube cubeCol;

	Ray rayCol;
};