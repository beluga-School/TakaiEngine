#include "TimeManager.h"


namespace TimeManager
{
	float deltaTime = 0;
	LARGE_INTEGER nowTime;
	LARGE_INTEGER time;
	LARGE_INTEGER nowTimeFreq;
}	

void TimeManager::Init()
{
	QueryPerformanceFrequency(&nowTimeFreq);
	QueryPerformanceCounter(&nowTime);
}

void TimeManager::Update()
{
	if (nowTimeFreq.QuadPart == 0) {
		return;
	}

	//処理直前の時間を算出
	QueryPerformanceCounter(&time);

	//処理時間を計算
	deltaTime = (float)(time.QuadPart - nowTime.QuadPart) / (float)nowTimeFreq.QuadPart;
	
	//次の処理直前の時間を代入
	nowTime = time;
}