#pragma once
#include "Entity.h"
	 
class RedCoin final : public Entity
{
public:
	RedCoin() : Entity()
	{
		//SetTag(TagTable::Coin);
	}

	void Initialize()override;
	void Update()override;
	void Draw()override;

	void HitEffect();

	//イベント名と一致しているスターを出現させる処理
	static void EventPopRedCoin(const std::string& eventname);

private:
	bool hit = false;
};

