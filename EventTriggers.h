#pragma once
#include <string>

struct EventTrigger
{
	//�C�x���g�g���K�[�̖��O
	std::string triggerName = "";

	//��������������Ă�����true�ɂȂ�
};

//�C�x���g�����s���邽�߂̏������W�߂ăt���O������N���X
class EventTriggers
{
public:
	static EventTriggers* Get()
	{
		static EventTriggers instance;
		return &instance;
	}

private:
	EventTriggers() {};
	~EventTriggers() {};
};

