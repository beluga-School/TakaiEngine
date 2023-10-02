#pragma once
#include "Vector2.h"
#include "TEasing.h"
#include <string>
#include <list>
#include "IEvent.h"
#include <memory>

class EventManager
{
public:
	//�C�x���g�J�n
	bool Start(const std::string& startEventName);

	//�C�x���g�������I��(nowEvent�𑦎�nullptr�ɂ���)
	void ForceEnd();

	//���ݎ��s���̃C�x���g���擾
	std::unique_ptr<IEvent>* GetNowEvent();

	//�C�x���g������C�x���g���擾
	std::unique_ptr<IEvent>* GetEvent(const std::string& eventName);

	static EventManager* Get()
	{
		static EventManager instance;
		return &instance;
	}

	void Initialize();

	void Update();
	
	//�C�x���g���p�̐���\��
	void Draw();

	//�o�^���ꂽ�C�x���g�����Z�b�g
	void Clear();

	template <class Event> void Register(const EventCamData& camdata,const std::string& eventName)
	{
		allEvents.emplace_back();
		allEvents.back() = std::make_unique<Event>();
		allEvents.back()->eventName = eventName;
		allEvents.back()->eventCamera.eventCamData = camdata;
		allEvents.back()->eventCamera.Initialize();
	}

private:
	EventManager(){};
	~EventManager(){};

	//�C�x���g�̏㉺�������Ȃ����āAend��ԂɈڍs
	void End();

	enum class State
	{
		None,
		Start,
		RunEvent,
		End,
	};
	
	std::list<std::unique_ptr<IEvent>> allEvents;
	std::unique_ptr<IEvent>* nowEvent = nullptr;

	TEasing::easeTimer startTimer = 0.5f;
	TEasing::easeTimer endTimer = 0.5f;

	Vector2 uppos{};
	Vector2 downpos{};

	State state = State::None;
};

