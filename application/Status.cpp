#include "Status.h"

std::list<Status*> StatusManager::statuses;

void StatusManager::Update()
{
	for (auto status : statuses)
	{
		status->Update();
	}
}

int32_t Status::GetOld()
{
	return mOld;
}

Status::Status(int32_t init) {
	mCurrent = init;
	mOld = init;
	//�}�l�[�W���[�ɓo�^
	Register();
};

Status::Status(const Status& rhs) {
	mCurrent = rhs.mCurrent;
	mOld = rhs.mOld;
	//�}�l�[�W���[�ɓo�^
	Register();
}

Status& Status::operator=(const Status& rhs) {
	mCurrent = rhs.mCurrent;
	mOld = rhs.mOld;
	//�}�l�[�W���[�ɓo�^
	Register();

	return *this;
};

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
	return mCurrent < mOld;
};

//�㏸�g���K�[
bool Status::IncreaseTrigger() {
	return mCurrent > mOld;
};

//�ω����Ȃ��t���O
bool Status::Constant() {
	return mCurrent == mOld;
};

void Status::Update()
{
	mOld = mCurrent;
}

void Status::Register()
{
	StatusManager::GetStatuses().push_back(this);
}
