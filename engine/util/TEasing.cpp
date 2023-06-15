#include <TEasing.h>
#include <TimeManager.h>
#include <math.h>
#include <MathF.h>

float TEasing::lerp(const float& start, const float& end, const float& timeRate)
{
	float easeVal = timeRate;
	return start * (1.0f - easeVal) + end * timeRate;
};

float TEasing::InQuad(const float& timeRate)
{
	return timeRate * timeRate;
};

float TEasing::OutQuad(const float& timeRate)
{
	return 1 - (1 - timeRate) * (1 - timeRate);
};

float TEasing::InQuad(const float& start, const float& end, const float& timeRate)
{
	float dif = end - start;
	dif *= InQuad(timeRate);
	return start + dif;
}
float TEasing::OutQuad(const float& start, const float& end, const float& timeRate)
{
	float dif = end - start;
	dif *= OutQuad(timeRate);
	return start + dif;
}

float TEasing::InOutQuad(const float& timeRate)
{
	return (float)(timeRate < 0.5f ? 2 * timeRate * timeRate : 1 - pow(-2 * timeRate + 2, 2) / 2);
}

float TEasing::InOutQuad(const float& start, const float& end, const float& timeRate)
{
	float dif = end - start;
	dif *= InOutQuad(timeRate);
	return start + dif;
}

float TEasing::OutBounce(const float& timeRate)
{
	const float n1 = 7.5625f;
	const float d1 = 2.75f;

	float temp = timeRate;

	if (timeRate < 1 / d1) {
		return n1 * timeRate * timeRate;
	}
	else if (timeRate < 2 / d1) {
		return n1 * (temp -= 1.5f / d1) * timeRate + 0.75f;
	}
	else if (timeRate < 2.5f / d1) {
		return n1 * (temp -= 2.25f / d1) * timeRate + 0.9375f;
	}
	else {
		return n1 * (temp -= 2.625f / d1) * timeRate + 0.984375f;
	}
}

float TEasing::OutBounce(const float& start, const float& end, const float& timeRate)
{
	float dif = end - start;
	dif *= OutBounce(timeRate);
	return start + dif;
}

float TEasing::InBounce(const float& start, const float& end, const float& timeRate){

	float dif = end - start;
	dif *= 1 - OutBounce(1 - timeRate);
	return start + dif;
}

float TEasing::InOutBounce(const float& start, const float& end, const float& timeRate)
{
	float dif = end - start;
	if (timeRate < 0.5)
	{
		dif *= (1 - OutBounce(1 - 2 * timeRate) / 2);
	}
	else
	{
		dif *= (1 + OutBounce(2 * timeRate - 1) / 2);
	}
	return start + dif;
}

float TEasing::InElastic(const float& timeRate)
{
	const float c4 = (2 * MathF::PIf) / 3;

	return timeRate == 0
		? 0
		: timeRate == 1
		? 1
		: -powf(2, 10 * timeRate - 10) * sinf((timeRate * 10 - 10.75f) * c4);
}

float TEasing::OutElastic(const float& timeRate)
{
	const float c4 = (2 * MathF::PIf) / 3;

	return timeRate == 0
		? 0
		: timeRate == 1
		? 1
		: powf(2, -10 * timeRate) * sinf((timeRate * 10 - 0.75f) * c4) + 1;
}

float TEasing::InElastic(const float& start, const float& end, const float& timeRate)
{
	float dif = end - start;
	dif *= InElastic(timeRate);
	return start + dif;
}

float TEasing::OutElastic(const float& start, const float& end, const float& timeRate)
{
	float dif = end - start;
	dif *= OutElastic(timeRate);
	return start + dif;
}

float TEasing::InBack(const float& timeRate)
{
	const float c1 = 1.70158f;
	const float c3 = c1 + 1;

	return c3 * timeRate * timeRate * timeRate - c1 * timeRate * timeRate;
}

float TEasing::InBack(const float& start, const float& end, const float& timeRate)
{
	float dif = end - start;
	dif *= InBack(timeRate);
	return start + dif;
}

float TEasing::OutBack(const float& timeRate)
{
	const float c1 = 1.70158f;
	const float c3 = c1 + 1;

	return 1 + c3 * powf(timeRate - 1, 3) + c1 * powf(timeRate - 1, 2);
}

float TEasing::OutBack(const float& start, const float& end, const float& timeRate)
{
	float dif = end - start;
	dif *= OutBack(timeRate);
	return start + dif;
}

float TEasing::InOutBack(const float& timeRate)
{
	const float c1 = 1.70158f;
	const float c2 = c1 * 1.525f;

	return timeRate < 0.5
		? (powf(2 * timeRate, 2) * ((c2 + 1) * 2 * timeRate - c2)) / 2
		: (powf(2 * timeRate - 2, 2) * ((c2 + 1) * (timeRate * 2 - 2) + c2) + 2) / 2;
}

float TEasing::InOutBack(const float& start, const float& end, const float& timeRate)
{
	float dif = end - start;
	dif *= InOutBack(timeRate);
	return start + dif;
}


void TEasing::easeTimer::Reset()
{
	mElapsedTime = 0.0f;
	end = false;
	reverseend = false;

	run = false;
	reverse = false;
}

void TEasing::easeTimer::Start()
{
	mElapsedTime = 0.0f;
	end = false;
	reverseend = false;

	run = true;
	reverse = false;
}

void TEasing::easeTimer::ReverseStart()
{
	end = false;
	reverseend = false;

	run = false;
	reverse = true;
}

void TEasing::easeTimer::Update()
{
	if (run) {
		mElapsedTime += TimeManager::deltaTime;
	}
	if (reverse) {
		mElapsedTime -= TimeManager::deltaTime;
	}
	// 経過時間が経過しきったらフラグを折る
	if (GetTimeRate() >= 1.0f && run) {
		run = false;
		end = true;
	}
	//中途半端なところで止まっちゃうけど、とりあえず0.1以下になったら止める
	if (GetTimeRate() <= 0.001f && reverse) {
		mElapsedTime = 0.0f;
		reverse = false;
		reverseend = true;
	}
}

float TEasing::easeTimer::GetTimeRate()
{
	return min(mElapsedTime / mMaxTime, 1.0f);
}