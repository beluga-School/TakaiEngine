#pragma once

namespace Util
{
	//�E�B���h�E�T�C�Y
	const float window_width = 1280;
	const float window_height = 720;

	template <class T>
	T Clamp(T t,T min, T max)
	{
		if (t < min) { return min; };
		if (t > max) { return max; };
		return t;
	}

    struct lerpVariables
    {
        //���݂̌o�ߎ���
        float elapsedTime = 0.0f;
        //���b�����Ĉړ����邩
        float maxTime = 1.0f;

        //�Ăяo������C�[�W���O���N��
        void Start();

        //�o�ߎ��Ԃ��X�V
        void Update();

        //���݂��ő�b���̂����ǂ̊�������(0.0f�`1.0f)�̒l�ŕԂ�
        float GetTimeRate();

    private:
        bool run = false;
    };
};

