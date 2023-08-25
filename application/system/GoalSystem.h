#pragma once
#include "Sprite.h"
#include "TEasing.h"

class ClearManage
{
public:
	static ClearManage* Get()
	{
		static ClearManage instance;
		return &instance;
	}

	//���O�ŃN���A�����X�e�[�W�̏���ۑ�����
	bool isClear = false;
	int32_t clearNumber = -1;
private:
	~ClearManage(){};
	ClearManage(){};
};

//�S�[����̏�����i�߂�N���X
//���̓X�v���C�g�o������
//����X�e�[�W�Ɏ������āA1�X�e�[�W=1�S�[�������A�Ƃ����`�ɂ�����
class GoalSystem
{
public:
	void LoadResource();

	void AnimetionStart();
	void GoalAnimetion();

	void Initialize();
	void Update();
	void Draw();

	//�N���A�t���O
	bool mIsClear = false;

private:
	Sprite mClearSprite;

	enum class AnimetionState
	{
		None,
		First,
		Second,
		Third,
	} animetionState = AnimetionState::None;

	TEasing::easeTimer firstTimer = 1.0f;
	TEasing::easeTimer secondTimer = 0.5f;
	TEasing::easeTimer thirdTimer = 1.0f;

};

