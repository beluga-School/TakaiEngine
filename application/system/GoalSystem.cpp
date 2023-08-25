#include "GoalSystem.h"
#include "Util.h"
#include "Stage.h"

void GoalSystem::LoadResource()
{
	TextureManager::Load("Resources\\goal.png", "goal");
}

void GoalSystem::AnimetionStart()
{
	if (animetionState != AnimetionState::None) return;
	firstTimer.Start();
	secondTimer.Reset();
	thirdTimer.Reset();
	animetionState = AnimetionState::First;
	mIsClear = true;

	//����ʂ����t���O�𗧂Ă�
	ClearManage::Get()->isClear = true;

	//�ʂ����X�e�[�W�̔ԍ����L�^����
	ClearManage::Get()->clearNumber =
		LevelLoader::Get()->GetData(StageChanger::Get()->GetNowStageHandle())->mStageNum;

}

void GoalSystem::GoalAnimetion()
{
	firstTimer.Update();
	secondTimer.Update();
	thirdTimer.Update();

	switch (animetionState)
	{
	case GoalSystem::AnimetionState::None:
		
		break;
	case GoalSystem::AnimetionState::First:
		mClearSprite.mPosition.y = TEasing::OutQuad(-100, Util::WIN_HEIGHT / 2, firstTimer.GetTimeRate());
		
		if (firstTimer.GetEnd())
		{
			animetionState = AnimetionState::Second;
			firstTimer.Reset();
			secondTimer.Start();
		}
		break;
	case GoalSystem::AnimetionState::Second:
		if (secondTimer.GetEnd())
		{
			animetionState = AnimetionState::Third;
			secondTimer.Reset();
			thirdTimer.Start();
		}

		break;
	case GoalSystem::AnimetionState::Third:
		mClearSprite.mPosition.y = TEasing::InQuad(Util::WIN_HEIGHT / 2, Util::WIN_HEIGHT + 100, thirdTimer.GetTimeRate());

		if (thirdTimer.GetEnd())
		{
			animetionState = AnimetionState::None;
			thirdTimer.Reset();
		
			//�ړ���̏����X�e�[�W�ֈڂ�
			StageChanger::Get()->saveNextDokanInfo.stageName = "stage_stageselect";
			//�����߂��Ă���X�e�[�W���Ƃɕς��Ȃ��Ƃ����Ȃ�
			StageChanger::Get()->saveNextDokanInfo.id = "0";

			//�X�e�[�W�Z���N�g�ɖ߂�
			StageChanger::Get()->ChangeLevel(*LevelLoader::Get()->GetData("stage_stageselect"));
		
			//����ʂ����t���O�𗧂Ă�
			ClearManage::Get()->isClear = true;

			//�ʂ����X�e�[�W�̔ԍ����L�^����
			ClearManage::Get()->clearNumber =
				LevelLoader::Get()->GetData(StageChanger::Get()->GetNowStageHandle())->mStageNum;
		}
		break;
	}
}

void GoalSystem::Initialize()
{
	mClearSprite.SetTexture(*TextureManager::GetTexture("goal"));
	mClearSprite.mPosition = { Util::WIN_WIDTH / 2,100,0 };
	mClearSprite.SetSize({ 176 * 2,32 * 2 });
}

void GoalSystem::Update()
{
	GoalAnimetion();

	mClearSprite.Update();
}

void GoalSystem::Draw()
{
	if (mIsClear)
	{
		mClearSprite.Draw();
	}
}

