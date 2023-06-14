#include "MathF.h"
#include <random>

namespace MathF
{
	// ランダムな値を取得
	float GetRand(const float& min,const float& max)
	{
		// 乱数生成器
		static std::random_device rd;
		static std::default_random_engine eng(rd());

		// 分布生成器
		std::uniform_real_distribution<float> get_rand_uni_real(min, max);

		// 乱数を生成
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
		float distanceX = x2 - x1;//距離を出す
		float dividedDistanceX = distanceX / flame;//距離をflameで割った値

		float temp = 0;
		temp += dividedDistanceX;//距離をflameで割った値を足す

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