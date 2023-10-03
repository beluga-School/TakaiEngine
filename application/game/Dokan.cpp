#include "Dokan.h"
#include "Input.h"
#include "Stage.h"
#include "SceneChange.h"
#include <GameUIManager.h>
#include <InstantDrawer.h>

void Dokan::LoadResource()
{
	TextureManager::Load("Resources\\ui\\LSHIFTdownUI.png", "LSHIFTdownUI");
}

void Dokan::Initialize()
{
	SetModel(ModelManager::GetModel("dokan"));
	SetTexture(TextureManager::GetTexture("white"));
}

void Dokan::Update()
{
	mPreMoveTimer.Update();
	mMainMoveTimer.Update();

	TargetEnter();

	UIUpdate();

	Obj3d::Update(*Camera::sCamera);
}

void Dokan::Draw()
{
	DrawMaterial();

	InstantDrawer::DrawGraph3D({
		position.x,
		position.y + 5,
		position.z,
		}, mUISize.x, mUISize.y, "LSHIFTdownUI");
}

void Dokan::HitEffect()
{
	
}

void Dokan::HitEffect(Mob* target_)
{
	//���łɊJ�n���Ă���Ȃ�X�L�b�v
	if (mDokanState != DokanState::None)return;

	//�{�^�����͂���������
	if (Input::Keyboard::TriggerKey(DIK_LSHIFT))
	{
		mPreMoveTimer.Start();
		mDokanState = DokanState::PreMove;
		mTarget = target_;
		//�^�[�Q�b�g�̓����蔻����Ȃ���
		mTarget->SetNoCollsion(true);
		mTarget->SetNoGravity(true);
		mTarget->SetNoMove(true);
		mSavePos = target_->box.position;

		//�ړ���̏����X�e�[�W�ֈڂ�
		StageChanger::Get()->saveNextDokanInfo.stageName = nextDokanInfo.stageName;
		StageChanger::Get()->saveNextDokanInfo.id = nextDokanInfo.id;

		//UI�𓮂���
		GameUIManager::Get()->Move(UIMove::END);
	}
}

void Dokan::TargetEnter()
{
	//�ێ������^�[�Q�b�g������Ȃ�
	if (mTarget != nullptr)
	{
		switch (mDokanState)
		{
		case Dokan::DokanState::None:
			break;
		case Dokan::DokanState::PreMove:

			//�^�[�Q�b�g�̏����ʒu��y�ǂ̐^��Ɉړ�
			mStartPos = { box.position.x ,box.position.y + box.scale.y / 2,box.position.z };
			mTarget->position = TEasing::InQuad(mSavePos, mStartPos, mPreMoveTimer.GetTimeRate());

			if (mPreMoveTimer.GetEnd())
			{
				mDokanState = DokanState::MainMove;
				mMainMoveTimer.Start();
			}

			break;
		case Dokan::DokanState::MainMove:
			//�y�ǂ̌��_�܂ňړ�������
			mTarget->position.y = TEasing::InQuad(mStartPos.y, box.position.y - scale.y / 2, mMainMoveTimer.GetTimeRate());

			if (mMainMoveTimer.GetEnd())
			{
				mDokanState = DokanState::End;
			}

			break;
		case Dokan::DokanState::End:
			//eventtriger�ɋL�ڂ��ꂽ�X�e�[�W�ֈړ�����
			std::vector<std::string> split = Util::SplitString(trigerName, "_");

			std::string stagename = "stage_";

			for (auto str : split)
			{
				//������"stage"�łȂ��Ȃ�ۑ�
				if (!Util::IsNumber(str) && str != "stage")
				{
					stagename += str;
				}
			}

			StageChanger::Get()->ChangeLevel(*LevelLoader::Get()->GetData(stagename));

			//���X�����ɖ߂�
			mDokanState = DokanState::None;

			//�ʒu���Œ�
			mTarget->position.y = box.position.y - scale.y / 2;

			break;
		}
	}
}

void Dokan::UIUpdate()
{
	popUIRangeSphere.center = position;
	
	mUIPopUpTimer.Update();
	mUIPopOutTimer.Update();

	switch (mUIState)
	{
	case Dokan::UIState::None:
		break;
	case Dokan::UIState::PopUp:
		mUISize.x = TEasing::OutBack(0.0f, 1.0f, mUIPopUpTimer.GetTimeRate());
		mUISize.y = TEasing::OutBack(0.0f, 1.0f, mUIPopUpTimer.GetTimeRate());
		
		if (mUIPopUpTimer.GetEnd())
		{
			mUIState = UIState::Exist;
		}

		break;
	case Dokan::UIState::Exist:
		break;
	case Dokan::UIState::PopOut:
		mUISize.x = TEasing::InBack(1.0f, 0.0f, mUIPopOutTimer.GetTimeRate());
		mUISize.y = TEasing::InBack(1.0f, 0.0f, mUIPopOutTimer.GetTimeRate());

		if (mUIPopOutTimer.GetEnd())
		{
			mUIState = UIState::None;
		}

		break;
	}
}

void Dokan::PopUpUI()
{
	//���o����ԈȊO�œ�������X�L�b�v
	if (mUIState != UIState::None)return;

	mUIPopUpTimer.Start();
	mUIState = UIState::PopUp;
}

void Dokan::PopOutUI()
{
	//�o����ԈȊO�œ�������X�L�b�v
	if (mUIState != UIState::Exist)return;

	mUIPopOutTimer.Start();
	mUIState = UIState::PopOut;
}
