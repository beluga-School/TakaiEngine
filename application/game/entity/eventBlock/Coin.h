#pragma once
#include "Entity.h"

/*! Coin
	コインオブジェクト(未使用)
*/
class Coin final : public Entity
{
public:
	Coin() : Entity()
	{
		SetTag(TagTable::Coin);
	}

	void Initialize()override;
	void Update()override;
	void Draw()override;
	void HitEffect();
private:
	bool hit = false;
};