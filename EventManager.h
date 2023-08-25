#pragma once
#include "Vector2.h"
#include "TEasing.h"
#include <string>
#include <list>

class EventManager
{
public:
	//�C�x���g�J�n
	static bool Start(const std::string& startEventName);
	//�C�x���g�I��
	static void End();

	//���݂̃C�x���g���擾
	std::string GetNowEvent();

	static EventManager* Get()
	{
		static EventManager instance;
		return &instance;
	}

	static void Initialize();

	static void Update();
	
	//�C�x���g���p�̐���\��
	static void Draw();

	//�C�x���g��o�^
	static void Register(const std::string& startEventName);

	static void Clear();

private:
	EventManager(){};
	~EventManager(){};

	enum class State
	{
		None,
		Start,
		End,
	};
	
	std::list<std::string> allEvents;

	//���ݔ��������Ă���C�x���g
	std::string nowEvent;

	static TEasing::easeTimer startTimer;
	static TEasing::easeTimer endTimer;

	static Vector2 uppos;
	static Vector2 downpos;

	static State state;
};

