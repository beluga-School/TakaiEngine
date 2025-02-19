#pragma once
#include "Entity.h"

/*! Star
	プレイヤーが取得できるスターのクラス
*/
class Star final : public Entity
{
public:
	Star() : Entity()
	{
		SetTag(TagTable::Star);
		SetTag(TagTable::DitherTransparent);
	}

	void Initialize()override;
	void Update()override;
	void Draw()override;

	//当たったときのやつ
	void HitEffect();
	//星の取得処理が終わっているか
	void StateEnd();
	//吸収処理の演出
	bool InHoleEnd();

	enum class StarState
	{
		None,
		jumpUp,
		Inhole,
		CountDown,
		End,
	};

	//現在の状態を取得
	StarState GetState();

	//すでに取得済みの場合に、状態を取得後に変化させる処理
	void SetCorrected();

	//新たにスターを出現させる処理
	static void PopStar(const Vector3& pop);

	//イベント名と一致しているスターを出現させる処理
	static Star* EventPopStar(const std::string& eventname);
	static void EventVanishStar(const std::string& eventname);
	static bool EventCheckStar(const std::string& eventname);
	
	//自身を消滅させる処理
	void Vanish();

	//UIに送る自身の状況(更新と描画はUI管理マネージャーで行う)
	class UIInfo
	{
	public:
		bool get = false;
		bool oldget = false;

		bool NowGet();

		void Update();

		Vector2 scale = { 0.5f,0.5f };
		float angle = 0;

	private:
		TEasing::easeTimer timer = 1.0f;
	};

	UIInfo uiInfo;

private:

public:
	int32_t id = -1;

private:
	bool hit = false;

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