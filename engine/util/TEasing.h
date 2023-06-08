#pragma once

namespace TEasing
{
	float lerp(float start, float end, float timeRate);
	
	float InQuad(float timeRate);
	float OutQuad(float timeRate);
	float InQuad(float start, float end,float timeRate);
	float OutQuad(float start, float end,float timeRate);

	float InOutQuad(float timeRate);
	float InOutQuad(float start,float end, float timeRate);

	float OutBounce(float timeRate);

	float OutBounce(float start, float end, float timeRate);
	float InBounce(float start, float end, float timeRate);
	float InOutBounce(float start, float end, float timeRate);

	float InElastic(float timeRate);
	float OutElastic(float timeRate);

	float InElastic(float start, float end, float timeRate);
	float OutElastic(float start, float end, float timeRate);

	float InBack(float timeRate);
	float InBack(float start, float end,float timeRate);

	float OutBack(float timeRate);
	float OutBack(float start, float end, float timeRate);

	float InOutBack(float timeRate);
	float InOutBack(float start, float end, float timeRate);

	struct easeTimer
	{
		easeTimer(float maxTime = 1.0f) {
			this->maxTime = maxTime;
		}

		float elapsedTime = 0.0f;	// 現在の経過時間
		float maxTime = 1.0f;		// 何秒かけて移動するか

		bool GetRun() { return run; };
		//一度でも起動されたらtrueになるフラグ
		bool GetStarted() { return run || end; };

		bool GetEnd() { return end; };
		void SetEnd(bool end_) { end = end_; };

		bool GetReverse() { return reverse; };
		bool GetReverseEnd() { return reverseend; };
		void SetReverseEnd(bool reverseend_) { reverseend = reverseend_; };

		bool GetReverseStarted() { return reverse || reverseend; };

		//イージングは起動しないが初期化する
		void Reset();

		// 呼び出したらイージングが起動
		void Start();

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