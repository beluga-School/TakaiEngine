#include "MathF.h"
#include <random>

namespace MathF
{
	// ランダムな値を取得
	float GetRand(float min, float max)
	{
		// 乱数生成器
		static std::random_device rd;
		static std::default_random_engine eng(rd());

		// 分布生成器
		std::uniform_real_distribution<float> get_rand_uni_real(min, max);

		// 乱数を生成
		return get_rand_uni_real(eng);
	}

	int GetRandInt(int min, int max)
	{
		// 乱数生成器
		static std::random_device rd;
		static std::default_random_engine eng(rd());

		// 分布生成器
		std::uniform_real_distribution<float> get_rand_uni_real(min, max);

		// 乱数を生成
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
		float distanceX = x2 - x1;//距離を出す
		float dividedDistanceX = distanceX / flame;//距離をflameで割った値

		x1 += dividedDistanceX;//距離をflameで割った値を足す

		return x1;
	}

	float EaseOutQuad(float num)
	{
		return 1 - (1 - num) * (1 - num);
	}
}