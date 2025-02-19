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
	mState = State::MOVE_IN;
	mFirstTimer.Start();

	mSizeX = 0;
	mPosX = 0;
}

void SceneChange::Initialize()
{
	mScene.SetTexture(*TextureManager::GetTexture("white"));
	mScene.mColor = { 0,0,0,1 };
	mScene.SetAnchor({ mRight,0 });

	mScene.mPosition = { 0,0 };
	mScene.SetSize({ 0,Util::WIN_HEIGHT });
}

void SceneChange::Update()
{
	mFirstTimer.Update();
	mSecondTimer.Update();
	
	switch (mState)
	{
	case SceneChange::State::MOVE_IN:
		mSizeX = TEasing::OutQuad(mRight, mLeft, mFirstTimer.GetTimeRate());
		if (mFirstTimer.GetEnd())
		{
			mState = State::INPUTSTANDBY;
		}

		break;
	case SceneChange::State::INPUTSTANDBY:
		//入力があるまで待機

		break;
	case SceneChange::State::MOVE_OUT:
		mPosX = TEasing::InQuad(mRight, mLeft, mSecondTimer.GetTimeRate());
		if (mSecondTimer.GetEnd())
		{
			mState = State::END;
		}
		break;
	case SceneChange::State::END:

		break;
	}

	mScene.mPosition = { mPosX,0 };
	mScene.SetSize({ mSizeX,Util::WIN_HEIGHT });

	mScene.Update();
}

void SceneChange::Draw()
{
	mScene.Draw();
}
