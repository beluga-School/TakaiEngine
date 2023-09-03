#include "GoalSystem.h"
#include "Util.h"
#include "Stage.h"
#include "EventManager.h"

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

	//�S�[���C�x���g���X�^�[�g
	EventManager::Get()->Start("goalCamera");

	//�S�[�����o���s�p�Ƀf�[�^����
	ClearManage::Get()->SetClearInfo(StageChanger::Get()->GetNowStageHandle());
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
		
			//�C�x���g�������I��
			EventManager::Get()->ForceEnd();

			//�X�e�[�W�Z���N�g�ɖ߂�
			StageChanger::Get()->ChangeLevel(*LevelLoader::Get()->GetData("stage_stageselect"));
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

void ClearManage::SetClearInfo(const std::string& stageName)
{
	//��񂪓����Ă���Ȃ�
	if (stageName != "")
	{
		//����ʂ����t���O�𗧂Ă�
		isClear = true;

		//�ʂ����X�e�[�W�̔ԍ����L�^����
		ClearManage::Get()->eventNumber =
			LevelLoader::Get()->GetData(stageName)->mStageNum;
	}
}
