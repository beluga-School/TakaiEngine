#pragma once
#include <stdint.h>

namespace Util
{
	//ウィンドウサイズ
	const uint32_t WIN_WIDTH = 1280;
	const uint32_t WIN_HEIGHT = 720;

	template <class T>
	T Clamp(T t,T min, T max)
	{
		if (t < min) { return min; };
		if (t > max) { return max; };
		return t;
	}
};

