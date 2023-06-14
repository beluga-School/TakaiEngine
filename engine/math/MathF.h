#pragma once
#include "Vector2.h"
#include "Vector3.h"

namespace MathF
{
	const float PIf = 3.141592654f;

	/// <summary>
	/// �����_���Ȓl���擾(float�^)
	/// </summary>
	/// <param name="min">�ŏ��l</param>
	/// <param name="max">�ő�l</param>
	float GetRand(const float& min,const float& max);

	/// <summary>
	/// �P�U���������l��Ԃ�
	/// </summary>
	/// <param name="time">�o�ߎ���</param>
	/// <param name="amplitude">�U��</param>
	/// <param name="period">����</param>
	/// <returns></returns>
	float SimpleHarmonicMotion(const float& time,const float& amplitude = 10.0f,const float& period = 60.0f);

	/// <summary>
	/// �~�^���������l��Ԃ�(���f����)
	/// </summary>
	/// <param name="CenterPoint">���S�_</param>
	/// <param name="radius">���S�_����̔��a</param>
	/// <param name="angle">�p�x</param>
	/// <returns></returns>
	Vector2 CircularMotion(const Vector2& CenterPoint,const float& radius,const float& angle);

	/// <summary>
	/// �⊮(����)
	/// </summary>
	/// <param name="x1">�ړ��������ϐ�</param>
	/// <param name="x2">�ړ�����������</param>
	/// <param name="flame">����</param>
	/// <returns>�⊮��̒l</returns>
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