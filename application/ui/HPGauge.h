#pragma once
#include "Sprite.h"
#include "Float4.h"
#include "Status.h"
#include "TEasing.h"

class Gauge
{
public:
	//�ʒu�ƕ�������ݒ�
	Gauge(const Vector2& pos, const int32_t& separetnum);

	void Initialize();
	void Update();
	void Draw();

	/// <summary>
	/// �Q�[�W�̃T�C�Y��ς���֐�
	/// </summary>
	/// <param name="currentNum">���ݒn������������</param>
	/// <param name="maxChange">true�ɂ���ƍő�l������������</param>
	void SetGaugeSize(int32_t currentNum,bool maxChange = false);

	/// <summary>
	/// �Q�[�W�̒l������������
	/// </summary>
	/// <param name="value">�l</param>
	void Addition(int32_t value);

	bool mIsVisilve = true;

	//�\�̐F
	Color mFlontColor = {0,1,0,1};
	//���̐F(�Q�[�W����ꂽ���̐F)
	Color mBackColor = {1,0,0,1};
	//�C���i�[�̐F(�ԃQ�[�W�̂���Ɍ��̐F)
	Color mInnerColor = { 0.1f,0.1f,0.1f,1};
	//�t���[���̐F
	Color mFrameColor = {0,0,0,1};

private:
	Vector2 mPos{};

	Sprite mFlontGauge;
	Sprite mBackGauge;

	Sprite mInner;
	Sprite mFrameGauge;

	//���݂̃Q�[�W��
	Status mGaugeNum = 0;
	//�Q�[�W�̕������̍ő�l
	int32_t mInitGaugeMax = 0;

	//�l1������̃Q�[�W�̑傫��
	float mGaugeSizeX = 0;

	//�Q�[�W�̍ő�T�C�Y
	const float GAUGE_MAX_SIZEX = 400.0f;

	//�Q�[�W�̏c�T�C�Y
	float mGaugeSizeY = 25.0f;

	//�t���[���̑傫���̔{��
	Vector2 mFrameSize = { 1.05f,1.5f };

	//����Ƃ��̃^�C�}�[
	TEasing::easeTimer mFlontTimer = 0.1f;
	//���鎞�Ɉ�u������傪����̂��C�ɂȂ�̂ŁA�P�\���Ԃ�t����
	TEasing::easeTimer mDelayTimer = 0.5f;

	TEasing::easeTimer mBackTimer = 0.75f;

	void GaugeMove();

	//�C�[�W���O�p�̂��낢��
	float mFlontStart = 0;
	float mFlontEnd = 0;
	float mBackStart = 0;
	float mBackEnd = 0;
};

