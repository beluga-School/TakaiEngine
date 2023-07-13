#pragma once
#include "Vector3.h"

namespace TEasing
{
	float lerp(const float& start, const float& end, const float& timeRate);
	
	float InQuad(const float& timeRate);
	float OutQuad(const float& timeRate);
	float InQuad(const float& start, const float& end, const float& timeRate);
	float OutQuad(const float& start, const float& end, const float& timeRate);
	Vector3 InQuad(const Vector3& start, const Vector3& end, const float& timeRate);
	Vector3 OutQuad(const Vector3& start, const Vector3& end, const float& timeRate);

	float InOutQuad(const float& timeRate);
	float InOutQuad(const float& start, const float& end, const float& timeRate);

	float OutBounce(const float& timeRate);

	float OutBounce(const float& start, const float& end, const float& timeRate);
	float InBounce(const float& start, const float& end, const float& timeRate);
	float InOutBounce(const float& start, const float& end, const float& timeRate);

	float InElastic(const float& timeRate);
	float OutElastic(const float& timeRate);

	float InElastic(const float& start, const float& end, const float& timeRate);
	float OutElastic(const float& start, const float& end, const float& timeRate);

	float InBack(const float& timeRate);
	float InBack(const float& start, const float& end, const float& timeRate);

	float OutBack(const float& timeRate);
	float OutBack(const float& start, const float& end, const float& timeRate);

	float InOutBack(const float& timeRate);
	float InOutBack(const float& start, const float& end, const float& timeRate);

	struct easeTimer
	{
		easeTimer(const float& maxTime_ = 1.0f) {
			mMaxTime = maxTime_;
		}

		float mElapsedTime = 0.0f;	// 現在の経過時間
		float mMaxTime = 1.0f;		// 何秒かけて移動するか

		bool GetRun() { return run; };
		//一度でも起動されたらtrueになるフラグ
		bool GetStarted() { return run || end; };

		bool GetEnd() { return end; };
		void SetEnd(const bool& end_) { end = end_; };

		bool GetReverse() { return reverse; };
		bool GetReverseEnd() { return reverseend; };
		void SetReverseEnd(const bool& reverseend_) { reverseend = reverseend_; };

		bool GetReverseStarted() { return reverse || reverseend; };

		//イージングは起動しないが初期化する
		void Reset();

		// 呼び出したらイージングが起動、タイマーを初期化する
		void Start();

		//イージングを逆方向に再生、現在の経過時間->0秒に向かって再生される
		void ReverseStart();

		// 経過時間を更新
		void Update();

		// 現在が最大秒数のうちどの割合かを(0.0f～1.0f)の値で返す
		float GetTimeRate();

	private:
		//起動中フラグ
		bool run = false;
		//終了フラグ
		bool end = false;

		bool reverse = false;
		bool reverseend = false;
	};
};