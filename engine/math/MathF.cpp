#include "MathF.h"
#include <random>

namespace MathF
{
	// �����_���Ȓl���擾
	float GetRand(const float& min,const float& max)
	{
		// ����������
		static std::random_device rd;
		static std::default_random_engine eng(rd());

		// ���z������
		std::uniform_real_distribution<float> get_rand_uni_real(min, max);

		// �����𐶐�
		return get_rand_uni_real(eng);
	}

	float SimpleHarmonicMotion(const float& time,const float& amplitude,const float& period)
	{
		return amplitude * sinf(2 * PIf * time / period);
	}

	Vector2 CircularMotion(const Vector2& CenterPoint,const float& radius,const float& angle)
	{
		Vector2 position;

		position.x = CenterPoint.x + radius * cosf(angle);
		position.y = CenterPoint.y + radius * sinf(angle);

		return position;
	}

	float Complement(const float& x1,const float& x2,const float& flame)
	{
		float distanceX = x2 - x1;//�������o��
		float dividedDistanceX = distanceX / flame;//������flame�Ŋ������l

		float temp = 0;
		temp += dividedDistanceX;//������flame�Ŋ������l�𑫂�

		return temp;
	}

	float GetAngleBetweenTwoLine(const Vector3& vec1,const Vector3& vec2)
	{
		return acosf(vec1.dot(vec2) / (vec1.length() * vec2.length()));
	}

	float AngleConvRad(const float& angle)
	{
		return PIf / 180 * angle;
	}

	float RadConvAngle(const float& rad) {
		return 180 / PIf * rad;
	}
}