#pragma once
#include "Enemy.h"

class Bombking : public Enemy
{
public:
	void Initialize()override;

	void Update()override;

	void Draw()override;

	void HitEffect()override;

	void Encount()override;

private:
	//‚±‚±‚àŒã‚ÅActTable‚É’u‚«Š·‚¦‚é
	enum class BossState
	{
		None,
		Encount,
		Tracking,
		Punch,
		HipDrop,
		CoolTime,
	}state = BossState::None;

	void Tracking();

	Vector3 pVec{};

	float mSpeed = 10.0f;

	Obj3d colDrawer;
};

