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
private:
	enum class DokanState
	{
		None,
		PreMove,
		MainMove,
		End,
	}mDokanState = DokanState::None;

	enum class UIState
	{
		None,
		PopUp,
		Exist,
		PopOut,
	}mUIState = UIState::None;

public:
	static void LoadResource();

	Dokan() : EventBlock()
	{
		SetTag(TagTable::Dokan);
	};

	~Dokan()
	{
		//�^�[�Q�b�g�̓����蔻��Ȃ�������������
		if (mTarget != nullptr)
		{
			mTarget->SetNoCollsion(false);
			mTarget->SetNoGravity(false);
			mTarget->SetNoMove(false);
		}
		//�^�[�Q�b�g�̕ێ�������
		mTarget = nullptr;
	}

	Mob* mTarget = nullptr;

	void Initialize();

	void Update();

	void Draw();

	void HitEffect();
	void HitEffect(Mob* target);

	//�Ăяo����UI���o������
	void PopUpUI();

	//�Ăяo����UI����������
	void PopOutUI();

	//�ړ���̏���ێ�
	DokanInfo nextDokanInfo;

	//���g�̏���ێ�
	DokanInfo dokanInfo;

	//������x�߂Â�����UI��\�����邽�߂̋�
	Sphere popUIRangeSphere = { {0,0,0} ,10.0f};

private:

	//�^�[�Q�b�g�����g�̒��ɓ����֐�
	void TargetEnter();

	//UI�̍X�V
	void UIUpdate();

	TEasing::easeTimer mPreMoveTimer = 0.25f;
	TEasing::easeTimer mMainMoveTimer = 0.75f;

	Vector3 mSavePos = {};
	Vector3 mStartPos = {};

	Vector2 mUISize = { 0,0 };

	TEasing::easeTimer mUIPopUpTimer = 0.5f;
	TEasing::easeTimer mUIPopOutTimer = 0.5f;
};

