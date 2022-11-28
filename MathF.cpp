#include "MathF.h"
#include <random>

namespace MathF
{
	// �����_���Ȓl���擾
	float GetRand(float min, float max)
	{
		// ����������
		static std::random_device rd;
		static std::default_random_engine eng(rd());

		// ���z������
		std::uniform_real_distribution<float> get_rand_uni_real(min, max);

		// �����𐶐�
		return get_rand_uni_real(eng);
	}

	int GetRandInt(int min, int max)
	{
		// ����������
		static std::random_device rd;
		static std::default_random_engine eng(rd());

		// ���z������
		std::uniform_real_distribution<float> get_rand_uni_real(min, max);

		// �����𐶐�
		return static_cast<int>(get_rand_uni_real(eng));
	}

	float SimpleHarmonicMotion(float time, float amplitude, float period)
	{
		return amplitude * sinf(2 * PI * time / period);
	}

	Vector2 CircularMotion(Vector2 CenterPoint, float radius, float angle)
	{
		Vector2 position;

		position.x = CenterPoint.x + radius * cosf(angle);
		position.y = CenterPoint.y + radius * sinf(angle);

		return position;
	}

	float Complement(float& x1, float& x2, float flame)
	{
		float distanceX = x2 - x1;//�������o��
		float dividedDistanceX = distanceX / flame;//������flame�Ŋ������l

		x1 += dividedDistanceX;//������flame�Ŋ������l�𑫂�

		return x1;
	}

	float EaseOutQuad(float num)
	{
		return 1 - (1 - num) * (1 - num);
	}
}