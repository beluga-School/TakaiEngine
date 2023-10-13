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

	uint32_t CenterX();
	uint32_t CenterY();

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

	void ChangeDebug();

	//stringを指定した区切り文字で分割する
	std::vector<std::string> SplitString(const std::string& str, const char* delimiter);

	//文字列に数値が含まれているか判定する
	bool IsNumber(const std::string& str);

	//文字列に指定した文字列が含まれているか判定する
	bool CheckString(const std::string& checkStr, const std::string& searchStr);
	//さらに見つけた文字列を返す
	std::string GetString(const std::string& checkStr, const std::string& searchStr);

	//文字列を指定した区切り文字で分割し、一番手前にあった文字を返す
	int32_t GetNumber(const std::string& str, const char* delimiter);

	//そのキーがマップ内に存在するかを検索する
	template <class Key, class Value>
	bool CheckFindEvent(std::unordered_map<Key, Value> map, Key key) {
		//存在するならtrueになり、存在しないならfalseになる
		return !(map.find(key) == map.end());
	}

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

