#pragma once
#include "Vector2.h"
#include "TEasing.h"

class EventManager
{
public:
	//�C�x���g�J�n
	static void Start();
	//�C�x���g�I��
	static void End();

	//���݂��C�x���g�����ǂ������擾
	static bool IsNowEvent();

	static void Initialize();

	static void Update();
	
	//�C�x���g���p�̐���\��
	static void Draw();

private:
	EventManager(){};
	~EventManager(){};

	enum class State
	{
		None,
		Start,
		End,
	};
	
	static TEasing::easeTimer startTimer;
	static TEasing::easeTimer endTimer;

	static Vector2 uppos;
	static Vector2 downpos;

	static State state;
};

