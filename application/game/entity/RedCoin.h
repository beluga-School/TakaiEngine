#pragma once
#include "Entity.h"
#include "TEasing.h"
	 
class RedCoin final : public Entity
{
public:
	RedCoin() : Entity()
	{
		SetTag(TagTable::RedCoin);
	}

	void Initialize()override;
	void Update()override;
	void Draw()override;

	//ヒット時の処理
	void HitEffect();

	//イベント名と一致しているスターを出現させる処理
	static void PopEventRedCoin(const std::string& eventname);

	//イベント名と一致しているスターがあるか
	static bool CheckEventRedCoin(const std::string& eventname);

	//イベント名と一致しているスターを消す
	static void DeleteEventRedCoin(const std::string& eventname);

private:
	bool hit = false;
	TEasing::easeTimer getTimer = 0.5f;

	Vector3 savePos = {};
};

