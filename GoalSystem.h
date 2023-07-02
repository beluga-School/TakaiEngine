#pragma once
#include "Sprite.h"

//�S�[����̏�����i�߂�N���X
//���̓X�v���C�g�o������
//����X�e�[�W�Ɏ������āA1�X�e�[�W=1�S�[�������A�Ƃ����`�ɂ�����
class GoalSystem
{
public:
	void LoadResource();

	void Initialize();
	void Update();
	void Draw();

	//�N���A�t���O
	bool mIsClear = false;

private:
	Sprite mClearSprite;

};

