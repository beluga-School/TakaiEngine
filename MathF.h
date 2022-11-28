#pragma once
#include"Vector2.h"
namespace MathF
{
	const float PI = 3.141592654f;

	/// <summary>
	/// �����_���Ȓl���擾(float�^)
	/// </summary>
	/// <param name="min">�ŏ��l</param>
	/// <param name="max">�ő�l</param>
	float GetRand(float min, float max);

	/// <summary>
	/// �����_���Ȓl���擾(int�^)
	/// </summary>
	/// <param name="min">�ŏ��l</param>
	/// <param name="max">�ő�l</param>
	int GetRandInt(int min, int max);

	/// <summary>
	/// �P�U���������l��Ԃ�
	/// </summary>
	/// <param name="time">�o�ߎ���</param>
	/// <param name="amplitude">�U��</param>
	/// <param name="period">����</param>
	/// <returns></returns>
	float SimpleHarmonicMotion(float time, float amplitude = 10.0f, float period = 60.0f);

	/// <summary>
	/// �~�^���������l��Ԃ�(���f����)
	/// </summary>
	/// <param name="CenterPoint">���S�_</param>
	/// <param name="radius">���S�_����̔��a</param>
	/// <param name="angle">�p�x</param>
	/// <returns></returns>
	Vector2 CircularMotion(Vector2 CenterPoint, float radius, float angle);

	/// <summary>
	/// �⊮(����)
	/// </summary>
	/// <param name="x1">�ړ��������ϐ�</param>
	/// <param name="x2">�ړ�����������</param>
	/// <param name="flame">����</param>
	/// <returns>�⊮��̒l</returns>
	float Complement(float& x1, float& x2, float flame);

	/// <summary>
	/// �C�[�W���O
	/// </summary>
	/// <param name="num">nowFlame/endFlame</param>
	/// <returns></returns>
	float EaseOutQuad(float num);
};