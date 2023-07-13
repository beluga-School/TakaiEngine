#include "SceneChange.h"
#include "Util.h"

SceneChange::State SceneChange::mState = State::NONE;

void SceneChange::LoadResource()
{
	
}

void SceneChange::Reset()
{
	mState = State::NONE;
}

void SceneChange::Start()
{
	mState = State::FIRST;
	mFirstTimer.Start();

	mSizeX = 0;
	mPosX = 0;
}

void SceneChange::Initialize()
{
	mScene.SetTexture(*TextureManager::GetTexture("white"));
	mScene.SetColor({ 0,0,0,1 });
	mScene.SetAnchor({ mRight,0 });

	mScene.SetPos({ 0,0 });
	mScene.SetSize({ 0,Util::WIN_HEIGHT });
}

void SceneChange::Update()
{
	mFirstTimer.Update();
	mSecondTimer.Update();
	
	switch (mState)
	{
	case SceneChange::State::FIRST:
		mSizeX = TEasing::OutQuad(mRight, mLeft, mFirstTimer.GetTimeRate());
		if (mFirstTimer.GetEnd())
		{
			mState = State::SECOND;
		}

		break;
	case SceneChange::State::SECOND:
		//“ü—Í‚ª‚ ‚é‚Ü‚Å‘Ò‹@

		break;
	case SceneChange::State::THIRD:
		mPosX = TEasing::InQuad(mRight, mLeft, mSecondTimer.GetTimeRate());
		if (mSecondTimer.GetEnd())
		{
			mState = State::END;
		}
		break;
	case SceneChange::State::END:

		break;
	}

	mScene.SetPos({ mPosX,0 });
	mScene.SetSize({ mSizeX,Util::WIN_HEIGHT });

	mScene.Update();
}

void SceneChange::Draw()
{
	mScene.Draw();
}
