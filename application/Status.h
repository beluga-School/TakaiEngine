#pragma once
#include <stdint.h>
#include <list>
#include <string>

struct Status
{
public:
	//���݂̃X�e�[�^�X
	int32_t mCurrent = -114514;

	int32_t GetOld();

	Status(int32_t init);

	Status(const Status& rhs);

	Status& operator=(const Status& rhs);

	~Status();

	//�����g���K�[
	bool DecreaseTrigger();
	//�㏸�g���K�[
	bool IncreaseTrigger();
	//�ω����Ȃ��t���O
	bool Constant();

	void Update();

private:
	//1�t���[���O�̃X�e�[�^�X
	int32_t mOld = -114514;

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
