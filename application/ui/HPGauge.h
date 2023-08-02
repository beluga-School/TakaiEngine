#pragma once
#include "Sprite.h"
#include "Float4.h"
#include "Status.h"
#include "TEasing.h"

class Gauge
{
public:
	//位置と分割数を設定
	Gauge(const Vector2& pos, const int32_t& separetnum);

	void Initialize();
	void Update();
	void Draw();

	/// <summary>
	/// ゲージのサイズを変える関数
	/// </summary>
	/// <param name="currentNum">現在地を書き換える</param>
	/// <param name="maxChange">trueにすると最大値も書き換える</param>
	void SetGaugeSize(int32_t currentNum,bool maxChange = false);

	/// <summary>
	/// ゲージの値を引数分足す
	/// </summary>
	/// <param name="value">値</param>
	void Addition(int32_t value);

	bool mIsVisilve = true;

	//表の色
	Color mFlontColor = {0,1,0,1};
	//裏の色(ゲージが削れた時の色)
	Color mBackColor = {1,0,0,1};
	//インナーの色(赤ゲージのさらに後ろの色)
	Color mInnerColor = { 0.1f,0.1f,0.1f,1};
	//フレームの色
	Color mFrameColor = {0,0,0,1};

private:
	Vector2 mPos{};

	Sprite mFlontGauge;
	Sprite mBackGauge;

	Sprite mInner;
	Sprite mFrameGauge;

	//現在のゲージ数
	Status mGaugeNum = 0;
	//ゲージの分割数の最大値
	int32_t mInitGaugeMax = 0;

	//値1あたりのゲージの大きさ
	float mGaugeSizeX = 0;

	//ゲージの最大サイズ
	const float GAUGE_MAX_SIZEX = 400.0f;

	//ゲージの縦サイズ
	float mGaugeSizeY = 25.0f;

	//フレームの大きさの倍率
	Vector2 mFrameSize = { 1.05f,1.5f };

	//削れるときのタイマー
	TEasing::easeTimer mFlontTimer = 0.1f;
	//減る時に一瞬先っちょが削れるのが気になるので、猶予時間を付ける
	TEasing::easeTimer mDelayTimer = 0.5f;

	TEasing::easeTimer mBackTimer = 0.75f;

	void GaugeMove();

	//イージング用のいろいろ
	float mFlontStart = 0;
	float mFlontEnd = 0;
	float mBackStart = 0;
	float mBackEnd = 0;
};

