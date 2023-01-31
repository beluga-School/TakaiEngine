#pragma once
#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")

namespace TimeManager
{
	extern float deltaTime;

	extern LARGE_INTEGER startTime;
	extern LARGE_INTEGER endTime;
	extern LARGE_INTEGER nowTimeFreq;

	extern float fps;
	extern float fixFPS;

	void Init();

	void Update();
}
