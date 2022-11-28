#pragma once
#include"Vector2.h"
namespace MathF
{
	const float PI = 3.141592654f;

	/// <summary>
	/// ランダムな値を取得(float型)
	/// </summary>
	/// <param name="min">最小値</param>
	/// <param name="max">最大値</param>
	float GetRand(float min, float max);

	/// <summary>
	/// ランダムな値を取得(int型)
	/// </summary>
	/// <param name="min">最小値</param>
	/// <param name="max">最大値</param>
	int GetRandInt(int min, int max);

	/// <summary>
	/// 単振動をした値を返す
	/// </summary>
	/// <param name="time">経過時間</param>
	/// <param name="amplitude">振幅</param>
	/// <param name="period">周期</param>
	/// <returns></returns>
	float SimpleHarmonicMotion(float time, float amplitude = 10.0f, float period = 60.0f);

	/// <summary>
	/// 円運動をした値を返す(複素数版)
	/// </summary>
	/// <param name="CenterPoint">中心点</param>
	/// <param name="radius">中心点からの半径</param>
	/// <param name="angle">角度</param>
	/// <returns></returns>
	Vector2 CircularMotion(Vector2 CenterPoint, float radius, float angle);

	/// <summary>
	/// 補完(等速)
	/// </summary>
	/// <param name="x1">移動したい変数</param>
	/// <param name="x2">移動したい距離</param>
	/// <param name="flame">時間</param>
	/// <returns>補完後の値</returns>
	float Complement(float& x1, float& x2, float flame);

	/// <summary>
	/// イージング
	/// </summary>
	/// <param name="num">nowFlame/endFlame</param>
	/// <returns></returns>
	float EaseOutQuad(float num);
};