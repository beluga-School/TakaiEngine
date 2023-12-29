#pragma once
#include "Enemy.h"

class BombSolider : public Enemy
{
public:
	BombSolider() : Enemy()
	{
		SetTag(TagTable::BombSolider);
	}

	void Initialize()override;
	void Update()override;
	void Draw()override;

	//ヒット時に発生すること
	void HitEffect()override;

	//エンカウント時の処理
	void Encount()override;

private:
	Vector3 pVec;

	TEasing::easeTimer scalingTimer = 1.0f;
	TEasing::easeTimer explosionTimer = 3.0f;
	//ダメージ判定が出る時間
	TEasing::easeTimer damageTimer = 0.5f;
	//パーティクルのクールタイム
	TEasing::easeTimer partCool = 0.05f;

	//爆発直前のスケール
	Vector3 def = { 1.0f,1.0f,1.0f };
};

