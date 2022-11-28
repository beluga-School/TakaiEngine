#pragma once
class Vector3
{
public:
	float x;	// x����
	float y;	// y����
	float z;	// z����
public:
	Vector3();
	Vector3(float x, float y, float z);

	//�����o�֐�

	/// <summary>
	/// ������Ԃ�
	/// </summary>
	float length() const;

	/// <summary>
	/// ���K��
	/// ���ʂ�return���A�{�̂����������Ȃ� �{�̂��������������ꍇ��normalize���g�p���Ă�������
	/// </summary>
	/// <param name="v"></param>
	/// <returns></returns>
	Vector3 GetNormalize();
	
	/// <summary>
	/// ���K��
	/// �{�̂�����������@�ϐ��ɑ���������ꍇ��GetNormalize���g�p���Ă������� 
	/// </summary>
	/// <param name="v"></param>
	/// <returns></returns>
	void normalize();
	
	float dot(const Vector3& v)const;

	/// <summary>
	/// ����
	/// �{�̂�����������@�ϐ��ɑ���������ꍇ��GetCross���g�p���Ă�������
	/// </summary>
	void cross(const Vector3& v);
	
	/// <summary>
	/// ����
	/// ���ʂ�return���A�{�̂����������Ȃ� �{�̂��������������ꍇ��cross���g�p���Ă�������
	/// </summary>
	Vector3 GetCross(const Vector3& v)const;

	Vector3 operator+()const;
	Vector3 operator-()const;

	Vector3& operator+=(const Vector3& v);
	Vector3& operator-=(const Vector3& v);
	Vector3& operator*=(float s);
	Vector3& operator/=(float s);
};

const Vector3 operator+(const Vector3& v1, const Vector3& v2);
const Vector3 operator-(const Vector3& v1, const Vector3& v2);
const Vector3 operator*(const Vector3& v1, float s);
const Vector3 operator*(float s, const Vector3& v1);
const Vector3 operator/(const Vector3& v1, float s);

/// <summary>
/// ���`�⊮(1���֐��⊮)
/// </summary>
/// <param name="start">�J�n�ʒu</param>
/// <param name="end">�I���ʒu</param>
/// <param name="t">���Ԃ̊���</param>
/// <returns></returns>
const Vector3 lerp(const Vector3& start, const Vector3& end, const float t);
