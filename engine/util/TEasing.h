#pragma once
#include "Vector3.h"

namespace TEasing
{
	float lerp(const float& start, const float& end, const float& timeRate);
	
	float InQuad(const float& timeRate);
	float OutQuad(const float& timeRate);
	float InQuad(const float& start, const float& end, const float& timeRate);
	float OutQuad(const float& start, const float& end, const float& timeRate);
	Vector3 InQuad(const Vector3& start, const Vector3& end, const float& timeRate);
	Vector3 OutQuad(const Vector3& start, const Vector3& end, const float& timeRate);

	float InOutQuad(const float& timeRate);
	float InOutQuad(const float& start, const float& end, const float& timeRate);

	float OutBounce(const float& timeRate);

	float OutBounce(const float& start, const float& end, const float& timeRate);
	float InBounce(const float& start, const float& end, const float& timeRate);
	float InOutBounce(const float& start, const float& end, const float& timeRate);

	float InElastic(const float& timeRate);
	float OutElastic(const float& timeRate);

	float InElastic(const float& start, const float& end, const float& timeRate);
	float OutElastic(const float& start, const float& end, const float& timeRate);

	float InBack(const float& timeRate);
	float InBack(const float& start, const float& end, const float& timeRate);

	float OutBack(const float& timeRate);
	float OutBack(const float& start, const float& end, const float& timeRate);

	float InOutBack(const float& timeRate);
	float InOutBack(const float& start, const float& end, const float& timeRate);

	struct easeTimer
	{
		easeTimer(const float& maxTime_ = 1.0f) {
			mMaxTime = maxTime_;
		}

		float mElapsedTime = 0.0f;	// ���݂̌o�ߎ���
		float mMaxTime = 1.0f;		// ���b�����Ĉړ����邩

		bool GetRun() { return run; };
		//��x�ł��N�����ꂽ��true�ɂȂ�t���O
		bool GetStarted() { return run || end; };

		bool GetEnd() { return end; };
		void SetEnd(const bool& end_) { end = end_; };

		bool GetReverse() { return reverse; };
		bool GetReverseEnd() { return reverseend; };
		void SetReverseEnd(const bool& reverseend_) { reverseend = reverseend_; };

		bool GetReverseStarted() { return reverse || reverseend; };

		//�C�[�W���O�͋N�����Ȃ�������������
		void Reset();

		// �Ăяo������C�[�W���O���N���A�^�C�}�[������������
		void Start();

		//�C�[�W���O���t�����ɍĐ��A���݂̌o�ߎ���->0�b�Ɍ������čĐ������
		void ReverseStart();

		// �o�ߎ��Ԃ��X�V
		void Update();

		// ���݂��ő�b���̂����ǂ̊�������(0.0f�`1.0f)�̒l�ŕԂ�
		float GetTimeRate();

	private:
		//�N�����t���O
		bool run = false;
		//�I���t���O
		bool end = false;

		bool reverse = false;
		bool reverseend = false;
	};
};