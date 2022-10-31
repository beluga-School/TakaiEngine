#pragma once
#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")

namespace TimeManager
{
	extern float deltaTime;

	extern LARGE_INTEGER nowTime;
	extern LARGE_INTEGER time;
	extern LARGE_INTEGER nowTimeFreq;

	void Init();

	void Update();
}
