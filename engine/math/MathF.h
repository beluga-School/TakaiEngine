#pragma once
#include "Vector2.h"
#include "Vector3.h"

namespace MathF
{
	const float PIf = 3.141592654f;

	/// <summary>
	/// ランダムな値を取得(float型)
	/// </summary>
	/// <param name="min">最小値</param>
	/// <param name="max">最大値</param>
	float GetRand(const float& min,const float& max);

	/// <summary>
	/// 単振動をした値を返す
	/// </summary>
	/// <param name="time">経過時間</param>
	/// <param name="amplitude">振幅</param>
	/// <param name="period">周期</param>
	/// <returns></returns>
	float SimpleHarmonicMotion(const float& time,const float& amplitude = 10.0f,const float& period = 60.0f);

	/// <summary>
	/// 円運動をした値を返す(複素数版)
	/// </summary>
	/// <param name="CenterPoint">中心点</param>
	/// <param name="radius">中心点からの半径</param>
	/// <param name="angle">角度</param>
	/// <returns></returns>
	Vector2 CircularMotion(const Vector2& CenterPoint,const float& radius,const float& angle);

	//入ってきた要素の平均を返す 要素全てが0だったら0を返す
	float Avarage(Vector3 v);

	/// <summary>
	/// 補完(等速)
	/// </summary>
	/// <param name="x1">移動したい変数</param>
	/// <param name="x2">移動したい距離</param>
	/// <param name="flame">時間</param>
	/// <returns>補完後の値</returns>
	float Complement(const float& x1,const float& x2,const float& flame);

	float GetAngleBetweenTwoLine(const Vector3& vec1,const Vector3& vec2);

	float AngleConvRad(const float& angle);

	float RadConvAngle(const float& rad);

	template <typename T>
	T Abs(T a)
	{
		if (a < 0)
		{
			a = -a;
		}
		return a;
	}
};