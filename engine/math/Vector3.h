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
	Vector3 GetNormalize()const;
	
	/// <summary>
	/// ���K��
	/// �{�̂�����������@�ϐ��ɑ���������ꍇ��GetNormalize���g�p���Ă������� 
	/// </summary>
	/// <param name="v"></param>
	/// <returns></returns>
	void normalize();
	
	/// <summary>
	/// ����
	/// </summary>
	/// <param name="v"></param>
	/// <returns></returns>
	float dot(const Vector3& v)const;
	
	/// <summary>
	/// �O��
	/// �{�̂�����������@�ϐ��ɑ���������ꍇ��GetCross���g�p���Ă�������
	/// </summary>
	void cross(const Vector3& v);
	
	/// <summary>
	/// �O��
	/// ���ʂ�return���A�{�̂����������Ȃ� �{�̂��������������ꍇ��cross���g�p���Ă�������
	/// </summary>
	Vector3 GetCross(const Vector3& v)const;

	/// <summary>
	/// �����ړ�����C�[�W���O
	/// </summary>
	/// <param name="start"></param>
	/// <param name="end"></param>
	/// <param name="t"></param>
	/// <returns></returns>
	static Vector3 lerp(const Vector3& start, const Vector3& end, const float t);

	static Vector3 easeInQuad(const Vector3& start, const Vector3& end, const float t);
	static Vector3 easeOutQuad(const Vector3& start, const Vector3& end, const float t);
	static Vector3 easeInOutQuad(const Vector3& start, const Vector3& end, const float t);

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