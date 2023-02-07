#include "Util.h"
#include "TimeManager.h"

void Util::lerpVariables::Start()
{
	elapsedTime = 0.0f;
	run = true;
}

void Util::lerpVariables::Update()
{
	if (run) {
		elapsedTime += TimeManager::deltaTime;
	}
	//経過時間が経過しきったらフラグを折る
	if (GetTimeRate() >= 1.0f) {
		run = false;
	}
}

float Util::lerpVariables::GetTimeRate()
{
	return min(elapsedTime / maxTime, 1.0f);
}