#pragma once

namespace Util
{
	//ウィンドウサイズ
	const float window_width = 1280;
	const float window_height = 720;

	template <class T>
	T Clamp(T t,T min, T max)
	{
		if (t < min) { return min; };
		if (t > max) { return max; };
		return t;
	}
};

