#pragma once
#include <stdint.h>
#include <list>
#include <string>

struct Status
{
public:
	//現在のステータス
	int32_t mCurrent = -114514;

	int32_t GetOld();

	Status(int32_t init);

	Status(const Status& rhs);

	Status& operator=(const Status& rhs);

	~Status();

	//減少トリガー
	bool DecreaseTrigger();
	//上昇トリガー
	bool IncreaseTrigger();
	//変化がないフラグ
	bool Constant();

	void Update();

private:
	//1フレーム前のステータス
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
