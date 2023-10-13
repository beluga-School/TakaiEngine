#include "StageTitleUI.h"
#include "Util.h"

void StageTitleUI::ChangeHandle(int32_t stagenum)
{
	stageTitle.SetTexture(*TextureManager::GetTexture(stageHandles[(int)stagenum]));
}

void StageTitleUI::Start()
{
	if (state == State::DisPlay)return;
	state = State::DisPlay;
	alphaTimer.Start();
	disPlayTimer.Reset();
}

bool StageTitleUI::GetEnd()
{
	return state == State::Vanish;
}

void StageTitleUI::LoadResource()
{
	TextureManager::Load("Resources\\stagename_1.png", "stageTitle_1");
	TextureManager::Load("Resources\\stagename_2.png", "stageTitle_2");
}

void StageTitleUI::Initialize()
{
	stageHandles.clear();
	//ステージ0はステージ選択なので空を入れておく
	stageHandles.push_back("stageTitle_1");
	stageHandles.push_back("stageTitle_1");
	stageHandles.push_back("stageTitle_2");

	stageTitle.mPosition = { Util::WIN_WIDTH / 2,Util::WIN_HEIGHT / 2 };
	stageTitle.mColor.f4.w = 0.0f;
}

void StageTitleUI::Update()
{
	alphaTimer.Update();
	disPlayTimer.Update();

	switch (state)
	{
	case StageTitleUI::State::None:
		
		break;
	case StageTitleUI::State::DisPlay:
		stageTitle.mColor.f4.w = TEasing::InQuad(0.0f,1.0f, alphaTimer.GetTimeRate());
		
		if (alphaTimer.GetEnd())
		{
			if (disPlayTimer.GetStarted() == false)
			{
				disPlayTimer.Start();
			}
		}
		if (disPlayTimer.GetEnd())
		{
			state = State::Vanish;
			alphaTimer.Start();
		}

		break;
	case StageTitleUI::State::Vanish:
		stageTitle.mColor.f4.w = TEasing::InQuad(1.0f, 0.0f, alphaTimer.GetTimeRate());

		break;
	}

	stageTitle.Update();
}

void StageTitleUI::Draw()
{
	stageTitle.Draw();
}