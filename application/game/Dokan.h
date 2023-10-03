#pragma once
#include "EventBlock.h"
#include "Box.h"
#include "Mob.h"
#include "ImguiManager.h"
#include <Billboard.h>

struct DokanInfo
{
	int32_t id = -1;
	std::string stageName = "";
};

class Dokan : public EventBlock
{
public:
	static void LoadResource();

	Dokan() : EventBlock()
	{
		SetTag(TagTable::Dokan);
	};

	~Dokan()
	{
		//ターゲットの当たり判定なくす処理を解除
		if (mTarget != nullptr)
		{
			mTarget->SetNoCollsion(false);
			mTarget->SetNoGravity(false);
			mTarget->SetNoMove(false);
		}
		//ターゲットの保持を解除
		mTarget = nullptr;
	}

	Mob* mTarget = nullptr;

	void Initialize();

	void Update();

	void Draw();

	void HitEffect();
	void HitEffect(Mob* target);

	//移動先の情報を保持
	DokanInfo nextDokanInfo;

	//自身の情報を保持
	DokanInfo dokanInfo;

private:
	enum class DokanState
	{
		None,
		PreMove,
		MainMove,
		End,
	}mDokanState = DokanState::None;

	TEasing::easeTimer mPreMoveTimer = 0.25f;
	TEasing::easeTimer mMainMoveTimer = 0.75f;

	Vector3 mSavePos = {};
	Vector3 mStartPos = {};
};

