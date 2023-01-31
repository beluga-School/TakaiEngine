#pragma once
class Vector2
{
public:
	float x;
	float y;

	Vector2(float x, float y);
	Vector2();

	/// <summary>
	/// ���������߂�֐�
	/// </summary>
	/// <returns>����</returns>
	/// ����const�́A�֐��̓����Ő��l�����������Ȃ��Ƃ����Ӗ���\��
	/// const�����邱�ƂŁA�@�\�I�ɂ����������邱�Ƃ͂ł��Ȃ��Ȃ�
	float GetLength() const;

	/// <summary>
	/// ���g�𐳋K������֐�
	/// </summary>
	void Normalized();

	/// <summary>
	/// ���K������֐�
	/// </summary>
	/// <returns>Vec2�̐��K�����ꂽ�l</returns>
	Vector2 GetNormalized() const;

	/// <summary>
	/// �O�ϋ��߂�֐�
	/// </summary>
	/// <param name="other"></param>
	/// <returns></returns>
	constexpr float Cross(const Vector2& other) const;

	/// <summary>
	/// ���ϋ��߂�֐�
	/// </summary>
	/// <param name="other"></param>
	/// <returns></returns>
	constexpr float Dot(const Vector2& other) const;

	//���g�����������Ȃ����
	Vector2 operator +(const Vector2& other) const;
	Vector2 operator -(const Vector2& other) const;
	Vector2 operator *(const Vector2& other) const;
	Vector2 operator /(const Vector2& other) const;

	Vector2 operator *(int& other) const;
	Vector2 operator /(int& other) const;

	Vector2 operator *(float& other) const;
	Vector2 operator /(float& other) const;

	Vector2 operator *(double& other) const;
	Vector2 operator /(double& other) const;

	//���g��������������
	Vector2 operator +=(const Vector2& other);
	Vector2 operator -=(const Vector2& other);
	Vector2 operator *=(const Vector2& other);
	Vector2 operator /=(const Vector2& other);

	Vector2 operator +=(int& scolor);
	Vector2 operator -=(int& scolor);
	Vector2 operator *=(int& scolor);
	Vector2 operator /=(int& scolor);

	Vector2 operator +=(float& scolor);
	Vector2 operator -=(float& scolor);
	Vector2 operator *=(float& scolor);
	Vector2 operator /=(float& scolor);

	Vector2 operator +=(double& scolor);
	Vector2 operator -=(double& scolor);
	Vector2 operator *=(double& scolor);
	Vector2 operator /=(double& scolor);
};