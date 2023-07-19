#pragma once
#include "EventBlock.h"

class Star : public EventBlock
{
public:
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void HitEffect()override;

	bool InHoleEnd() {
		return timer.GetEnd();
	}

	enum class StarState
	{
		None,
		Inhole,
		End,
	}starState = StarState::None;

private:

	//入手時に保存するスケール
	Vector3 saveScale = { 0,0,0 };

	//入手時にプレイヤーに吸い込まれるイージングの始点と終点
	Vector3 saveStartPos = { 0,0,0 };
	Vector3 savePlayerPos = { 0,0,0 };

	//1秒間の回転数
	float rotaSpeed = 0.25f;

	TEasing::easeTimer timer = 2.0f;

	TEasing::easeTimer particleTimer = 1.0f;
};

class StarManager
{
public:
	//スターはプレイヤーとの結びつきが強いので、別で管理するように
	//今後ステージごとに、ステージデータとは別のファイルで星の取得状況とかを管理する用にしよう
	std::list<std::unique_ptr<Star>> mStars;

	//星取得演出の進行管理
	enum class StarGetState
	{
		None,	//何もない
		MoveCam,//カメラが動く
		Inhole,	//プレイヤーに吸い込まれる
		Jumping,//プレイヤーがジャンプする
		BackCam,//カメラが元に戻る
	}progress = StarGetState::None;

	static StarManager* Get() {
		static StarManager instance;
		return &instance;
	};

	void Start() {
		progress = StarGetState::MoveCam;
		jumpingCount = 0;
	};

	void JumpMove();

	void Update();

private:
	StarManager(){};
	~StarManager(){};

	TEasing::easeTimer jumpingTimer = 1.0f;
	int32_t jumpingCount = 0;
};
