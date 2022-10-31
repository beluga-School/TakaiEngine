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

	//�������O�̎��Ԃ��Z�o
	QueryPerformanceCounter(&time);

	//�������Ԃ��v�Z
	deltaTime = (float)(time.QuadPart - nowTime.QuadPart) / (float)nowTimeFreq.QuadPart;
	
	//���̏������O�̎��Ԃ���
	nowTime = time;
}