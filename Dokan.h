#pragma once
#include "EventBlock.h"
#include "Box.h"
#include "Mob.h"
#include "ImguiManager.h"

struct DokanInfo
{
	std::string id = "";
	std::string stageName = "";
};

class Dokan : public EventBlock
{
public:
	Dokan() : EventBlock()
	{
		taglist.push_back(TagTable::Dokan);
		taglist.push_back(TagTable::Block);
	};

	~Dokan()
	{
		//ターゲットの当たり判定なくす処理を解除
		if (mTarget != nullptr)
		{
			mTarget->SetNoCollsion(false);
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

