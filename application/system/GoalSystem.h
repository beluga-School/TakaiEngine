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

	//�S�[��������S�[�����o�����s���邽�߂̏��
	bool isClear = false;
	int32_t eventNumber = -1;

	void SetClearInfo(const std::string& stageName = "");

	//����ł悭�Ȃ��H�H�H�H�H�H�H�H�H�H�H�H�H�H�H�H�H�H�H
	//���������߂�true�ɂȂ����Ƃ��ɉ�����o������
	bool stage1clear = 0;
	bool stage2clear = 0;
	bool stage3clear = 0;
	bool stage4clear = 0;
	bool stage5clear = 0;
	bool stage6clear = 0;

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

