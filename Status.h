#pragma once
#include <stdint.h>
#include <list>

struct Status
{
public:
	//���݂̃X�e�[�^�X
	int32_t current = -114514;
	//1�t���[���O�̃X�e�[�^�X
	int32_t old = -114514;

	Status(int32_t init) {
		current = init;
		old = init;
		//�}�l�[�W���[�ɓo�^
		Register();
	};

	Status(const Status& rhs) {
		current = rhs.current;
		old = rhs.old;
		//�}�l�[�W���[�ɓo�^
		Register();
	}

	Status& operator=(const Status& rhs) {
		current = rhs.current;
		old = rhs.old;
		//�}�l�[�W���[�ɓo�^
		Register();
	};

	~Status();

	//�����g���K�[
	bool DecreaseTrigger();
	//�㏸�g���K�[
	bool IncreaseTrigger();
	//�ω����Ȃ��t���O
	bool Constant();

	void Update();

private:
	void Register();
};

class StatusManager
{
public:
	static void Update();

	static std::list<Status*>& GetStatuses()
	{
		return statuses;
	}

private:
	static std::list<Status*> statuses;
};
