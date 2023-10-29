#pragma once
#include <Vector2.h>
#include <TEasing.h>
#include <Sprite.h>
#include <Util.h>

/*! SceneChange
	シーン切り替え時の暗転を行うクラス
*/
class SceneChange
{
public:
	static void LoadResource();

	void Reset();

	void Start();
	void Initialize();
	void Update();
	void Draw();

	static bool GetEnd()
	{
		return mState == State::END;
	}

	static bool GetRun()
	{
		return mState != State::NONE && mState != State::END;
	}

	static bool IsBlackOut()
	{
		return mState == State::INPUTSTANDBY;
	}

	void Open()
	{
		if (mState == State::INPUTSTANDBY)
		{
			mSecondTimer.Start();
			mState = State::MOVE_OUT;
		}
	}

	static SceneChange* Get()
	{
		static SceneChange instance;
		return &instance;
	}

private:
	Sprite mScene;

	enum class State
	{
		NONE,
		MOVE_IN,
		INPUTSTANDBY,
		MOVE_OUT,
		END,
	};
	static State mState;

	float mSizeX = 0;
	float mPosX = 0;

	const float mRight = 0;
	const float mLeft = Util::WIN_WIDTH;

	TEasing::easeTimer mFirstTimer = 0.25f;
	TEasing::easeTimer mSecondTimer = 0.25f;

	SceneChange() {};
	~SceneChange() {};
};
