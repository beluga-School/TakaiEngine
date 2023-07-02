#include "GoalSystem.h"
#include "Util.h"

void GoalSystem::LoadResource()
{
}

void GoalSystem::Initialize()
{
	mClearSprite.SetTexture(*TextureManager::GetTexture("slime"));
	mClearSprite.mPosition = { Util::WIN_WIDTH / 2,0,0 };
}

void GoalSystem::Update()
{
	mClearSprite.Update();
}

void GoalSystem::Draw()
{
	if (mIsClear)
	{
		mClearSprite.Draw();
	}
}

