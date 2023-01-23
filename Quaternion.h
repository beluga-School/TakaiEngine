#pragma once
#include "Vector3.h"
#include "Matrix4.h"

class Quaternion
{
public:
	Vector3 vector;
	float w;

	Quaternion() {
		IdentityQuaternion();
	};

	Quaternion(float x,float y,float z,float w) {
		vector = { x,y,z };
		this->w = w;
	};

	//�P�ʃN�H�[�^�j�I��
	static Quaternion IdentityQuaternion();

	/// <summary>
	/// �����N�H�[�^�j�I��
	/// �{�̂�����������
	/// </summary>
	void Conjugate();

	/// <summary>
	/// �����N�H�[�^�j�I��
	/// ��������肷��
	/// </summary>
	/// <returns></returns>
	Quaternion GetConjugate();

	float GetNorm()const;

	void Normalize();
	Quaternion GetNormalize();

	//�N�H�[�^�j�I���Ƃ̊|���Z
	Quaternion operator*(Quaternion& r);
	
	Quaternion GetInverse();

	Vector3 RotateVector(const Vector3& vector);

	Matrix4 MakeRotateMatrix();
};

Quaternion MakeAxisAngle(const Vector3& axis, float angle);
