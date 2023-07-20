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
	return current < old;
};

//上昇トリガー
bool Status::IncreaseTrigger() {
	return current > old;
};

//変化がないフラグ
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
