#include "TimeManager.h"

namespace TimeManager
{
	float deltaTime = 0;
	float fps = 0;
	float fixFPS = 60;
	LARGE_INTEGER startTime;
	LARGE_INTEGER endTime;
	LARGE_INTEGER nowTimeFreq;
}	

//const float MIN_FREAM_TIME = 1.0f / (TimeManager::fixFPS);

void TimeManager::Init()
{
	QueryPerformanceFrequency(&nowTimeFreq);
	QueryPerformanceCounter(&startTime);
}

void TimeManager::Update()
{
	if (nowTimeFreq.QuadPart == 0) {
		return;
	}

	//�������O�̎��Ԃ��Z�o
	QueryPerformanceCounter(&endTime);

	{
		float flameTime = (float)(endTime.QuadPart - startTime.QuadPart) / (float)nowTimeFreq.QuadPart;
		float MIN_FREAM_TIME = 1.0f / (TimeManager::fixFPS);

		if (flameTime < MIN_FREAM_TIME)
		{
			DWORD sleepTime = static_cast<DWORD>((MIN_FREAM_TIME - flameTime) * 1000);

			timeBeginPeriod(1);
			Sleep(sleepTime);
			timeEndPeriod(1);

		}
		QueryPerformanceCounter(&endTime);
	}

	//�������Ԃ��v�Z
	deltaTime = (float)(endTime.QuadPart - startTime.QuadPart) / (float)nowTimeFreq.QuadPart;

	if (deltaTime > 0)
	{
		fps = (fps * 0.99f) + (0.01f / deltaTime);
	}
	
	//���̏������O�̎��Ԃ���
	startTime = endTime;
}