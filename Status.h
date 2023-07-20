#pragma once
#include <stdint.h>
#include <list>

struct Status
{
public:
	//現在のステータス
	int32_t current = -114514;
	//1フレーム前のステータス
	int32_t old = -114514;

	Status(int32_t init) {
		current = init;
		old = init;
		//マネージャーに登録
		Register();
	};

	Status(const Status& rhs) {
		current = rhs.current;
		old = rhs.old;
		//マネージャーに登録
		Register();
	}

	Status& operator=(const Status& rhs) {
		current = rhs.current;
		old = rhs.old;
		//マネージャーに登録
		Register();
	};

	~Status();

	//減少トリガー
	bool DecreaseTrigger();
	//上昇トリガー
	bool IncreaseTrigger();
	//変化がないフラグ
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
