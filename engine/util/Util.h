#pragma once
#include <stdint.h>
#include "Vector2.h"
#include <vector>
#include <string>
#include <unordered_map>

namespace Util
{
	//�E�B���h�E�T�C�Y
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

	//string���w�肵����؂蕶���ŕ�������
	std::vector<std::string> SplitString(const std::string& str, const char* delimiter);

	//������ɐ��l���܂܂�Ă��邩���肷��
	bool IsNumber(const std::string& str);

	//������Ɏw�肵�������񂪊܂܂�Ă��邩���肷��
	bool CheckString(const std::string& checkStr, const std::string& searchStr);
	//����Ɍ������������Ԃ�
	std::string GetString(const std::string& checkStr, const std::string& searchStr);

	//��������w�肵����؂蕶���ŕ������A��Ԏ�O�ɂ�����������Ԃ�
	int32_t GetNumber(const std::string& str, const char* delimiter);

	//���̃L�[���}�b�v���ɑ��݂��邩����������
	template <class Key, class Value>
	bool CheckFindEvent(std::unordered_map<Key, Value> map, Key key) {
		//���݂���Ȃ�true�ɂȂ�A���݂��Ȃ��Ȃ�false�ɂȂ�
		return !(map.find(key) == map.end());
	}

	//�}�b�v����n���h�����擾����
	//�R�s�[���Ă�̂ŏ������x�I����Ă�
	//�|�C���^�ɂ���ƃG���[�f����
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

