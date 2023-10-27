#pragma once
#include "EventBlock.h"

/*! Star
	プレイヤーが取得できるスターのクラス
*/
class Star final : public EventBlock
{
public:
	Star() : EventBlock()
	{
		SetTag(TagTable::Star);
	}

	void Initialize()override;
	void Update()override;
	void Draw()override;
	void HitEffect()override;

	//ステートがCountUpまで到達していたら、Endに変更する
	void StateEnd();

	bool InHoleEnd();

	enum class StarState
	{
		None,
		jumpUp,
		Inhole,
		CountUp,
		End,
	};

	//現在の状態を取得
	StarState GetState();

	//すでに取得済みの場合に、状態を取得後に変化させる処理
	void SetCorrected();

	int32_t id = -1;

private:
	StarState starState = StarState::None;

	//入手時に保存するスケール
	Vector3 saveScale = { 0,0,0 };

	//入手時にプレイヤーに吸い込まれるイージングの始点と終点
	Vector3 saveStartPos = { 0,0,0 };
	Vector3 savePlayerPos = { 0,0,0 };

	Vector3 jumpUpPos = { 0,0,0 };
	Vector3 jumpUpScale = { 0,0,0 };

	//1秒間の回転数
	float rotaSpeed = 0.25f;

	//飛び上がる時間
	TEasing::easeTimer jumpUpTimer = 1.0f;
	//空中でとどまる時間
	TEasing::easeTimer delayTimer = 0.2f;
	//吸い込まれる時間
	TEasing::easeTimer inholeTimer = 2.0f;

	//パーティクルの発生タイマー
	TEasing::easeTimer particleTimer = 1.0f;
};