#pragma once
#include <stdint.h>
#include "Vector2.h"
#include <vector>
#include <string>
#include <unordered_map>

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

	//stringを指定した区切り文字で分割する
	std::vector<std::string> SplitString(const std::string& str, const char* delimiter);

	//文字列に数値が含まれているか判定する
	bool IsNumber(const std::string& str);

	//マップからハンドルを取得する
	//コピーしてるので処理速度終わってる
	//ポインタにするとエラー吐いた
	template <class Key, class Value>
	std::vector<Key> GetKeys(std::unordered_map<Key, Value> map) {
		std::vector<Key> temp;

		for (std::pair<Key, Value> key : map)
		{
			temp.push_back(key.first);
		}
		return temp;
	};
};

