#pragma once

namespace TEasing
{
	float lerp(float start, float end, float timeRate);
	
	float InQuad(float timeRate);
	float OutQuad(float timeRate);
	float InQuad(float start, float end,float timeRate);
	float OutQuad(float start, float end,float timeRate);

	float InOutQuad(float timeRate);
	float InOutQuad(float start,float end, float timeRate);

	float OutBounce(float timeRate);

	float OutBounce(float start, float end, float timeRate);
	float InBounce(float start, float end, float timeRate);
	float InOutBounce(float start, float end, float timeRate);

	float InElastic(float timeRate);
	float OutElastic(float timeRate);

	float InElastic(float start, float end, float timeRate);
	float OutElastic(float start, float end, float timeRate);

	float InBack(float timeRate);
	float InBack(float start, float end,float timeRate);

	float OutBack(float timeRate);
	float OutBack(float start, float end, float timeRate);

	float InOutBack(float timeRate);
	float InOutBack(float start, float end, float timeRate);

	struct easeTimer
	{
		easeTimer(float maxTime = 1.0f) {
			this->maxTime = maxTime;
		}

		float elapsedTime = 0.0f;	// ���݂̌o�ߎ���
		float maxTime = 1.0f;		// ���b�����Ĉړ����邩

		bool GetRun() { return run; };
		//��x�ł��N�����ꂽ��true�ɂȂ�t���O
		bool GetStarted() { return run || end; };

		bool GetEnd() { return end; };
		void SetEnd(bool end_) { end = end_; };

		bool GetReverse() { return reverse; };
		bool GetReverseEnd() { return reverseend; };
		void SetReverseEnd(bool reverseend_) { reverseend = reverseend_; };

		bool GetReverseStarted() { return reverse || reverseend; };

		//�C�[�W���O�͋N�����Ȃ�������������
		void Reset();

		// �Ăяo������C�[�W���O���N��
		void Start();

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