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
	//マネージャーに登録
	Register();
};

Status::Status(const Status& rhs) {
	mCurrent = rhs.mCurrent;
	mOld = rhs.mOld;
	//マネージャーに登録
	Register();
}

Status& Status::operator=(const Status& rhs) {
	mCurrent = rhs.mCurrent;
	mOld = rhs.mOld;
	//マネージャーに登録
	Register();

	return *this;
};

Status::~Status()
{
	for (auto itr = StatusManager::GetStatuses().begin();itr != StatusManager::GetStatuses().end();)
	{
		if (*itr == this)
		{
			//eraseは次のポインタを返すので、itrに代入することでitr++と同じ挙動をしながら要素を消せるぜ
			itr = StatusManager::GetStatuses().erase(itr);
		}
		else
		{
			//それ以外はいつも通り進めるぜ
			itr++;
		}
	}
}

//減少トリガー
bool Status::DecreaseTrigger() {
	return mCurrent < mOld;
};

//上昇トリガー
bool Status::IncreaseTrigger() {
	return mCurrent > mOld;
};

//変化がないフラグ
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
