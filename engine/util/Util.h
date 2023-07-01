#pragma once
#include <stdint.h>
#include "Vector2.h"

namespace Util
{
	//ウィンドウサイズ
	const uint32_t WIN_WIDTH = 1280;
	const uint32_t WIN_HEIGHT = 720;

	bool OutOfScreen(const Vector2& pos);

	template <class T>
	T Clamp(T t,T min, T max)
	{
		if (t < min) { return min; };
		if (t > max) { return max; };
		return t;
	}

	template <class T>
	T Abs(T value)
	{
		if (value < 0)value *= -1;
		return value;
	}
};

