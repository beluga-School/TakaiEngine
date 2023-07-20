#include "Status.h"

std::list<Status*> StatusManager::statuses;

void StatusManager::Update()
{
	for (auto status : statuses)
	{
		status->Update();
	}
}

Status::~Status()
{
	for (auto itr = StatusManager::GetStatuses().begin();itr != StatusManager::GetStatuses().end();)
	{
		if (*itr == this)
		{
			//erase�͎��̃|�C���^��Ԃ��̂ŁAitr�ɑ�����邱�Ƃ�itr++�Ɠ������������Ȃ���v�f�������邺
			itr = StatusManager::GetStatuses().erase(itr);
		}
		else
		{
			//����ȊO�͂����ʂ�i�߂邺
			itr++;
		}
	}
}

//�����g���K�[
bool Status::DecreaseTrigger() {
	return current < old;
};

//�㏸�g���K�[
bool Status::IncreaseTrigger() {
	return current > old;
};

//�ω����Ȃ��t���O
bool Status::Constant() {
	return current == old;
};

void Status::Update()
{
	old = current;
}

void Status::Register()
{
	StatusManager::GetStatuses().push_back(this);
}
