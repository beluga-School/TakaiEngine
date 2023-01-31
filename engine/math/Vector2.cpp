#include "Vector2.h"
#include <math.h>

//Vec2
Vector2::Vector2(float x, float y) : x(x), y(y)
{
}

Vector2::Vector2() : x(0), y(0)
{
}

float Vector2::GetLength() const
{
	//const������Ƃ����������Ɓ����ł��Ȃ��Ȃ� �܂�ݾ��!!
	//                       x = 10; 
	return sqrtf(x * x + y * y);
}

void Vector2::Normalized()
{
	float length = GetLength();
	x = x / length;
	y = y / length;
}

Vector2 Vector2::GetNormalized() const
{
	Vector2 v;
	float length = GetLength();
	v.x = x / length;
	v.y = y / length;
	return v;
}

constexpr float Vector2::Cross(const Vector2& other) const
{
	return x * other.y - y * other.x;
}

constexpr float Vector2::Dot(const Vector2& other) const
{
	return x * other.x + y * other.y;
}

//���g�����������Ȃ����
Vector2 Vector2::operator +(const Vector2& other) const
{
	Vector2 v;
	v.x = x + other.x;
	v.y = y + other.y;
	return v;
}

Vector2 Vector2::operator -(const Vector2& other) const
{
	Vector2 v;
	v.x = x - other.x;
	v.y = y - other.y;
	return v;
}

Vector2 Vector2::operator *(const Vector2& other) const
{
	Vector2 v;
	v.x = x * other.x;
	v.y = y * other.y;
	return v;
}

Vector2 Vector2::operator /(const Vector2& other) const
{
	Vector2 v;
	v.x = x / other.x;
	v.y = y / other.y;
	return v;
}

Vector2 Vector2::operator *(int& other) const
{
	Vector2 v;
	v.x = x * other;
	v.y = y * other;
	return v;
}

Vector2 Vector2::operator /(int& other) const
{
	Vector2 v;
	v.x = x / other;
	v.y = y / other;
	return v;
}

Vector2 Vector2::operator *(float& other) const
{
	Vector2 v;
	v.x = x * other;
	v.y = y * other;
	return v;
}

Vector2 Vector2::operator /(float& other) const
{
	Vector2 v;
	v.x = x / other;
	v.y = y / other;
	return v;
}

Vector2 Vector2::operator *(double& other) const
{
	Vector2 v;
	v.x = x * other;
	v.y = y * other;
	return v;
}

Vector2 Vector2::operator /(double& other) const
{
	Vector2 v;
	v.x = x / other;
	v.y = y / other;
	return v;
}

//���g��������������
Vector2 Vector2::operator +=(const Vector2& other)
{
	x += other.x;
	y += other.y;

	return *this;
}

Vector2 Vector2::operator -=(const Vector2& other)
{
	x -= other.x;
	y -= other.y;

	return *this;
}

Vector2 Vector2::operator *=(const Vector2& other)
{
	x *= other.x;
	y *= other.y;

	return *this;
}

Vector2 Vector2::operator /=(const Vector2& other)
{
	x /= other.x;
	y /= other.y;

	return *this;
}

Vector2 Vector2::operator +=(int& scolor)
{
	x += scolor;
	y += scolor;

	return *this;
}

Vector2 Vector2::operator -=(int& scolor)
{
	x -= scolor;
	y -= scolor;

	return *this;
}

Vector2 Vector2::operator *=(int& scolor)
{
	x *= scolor;
	y *= scolor;

	return *this;
}

Vector2 Vector2::operator /=(int& scolor)
{
	x /= scolor;
	y /= scolor;

	return *this;
}

Vector2 Vector2::operator +=(float& scolor)
{
	x += scolor;
	y += scolor;

	return *this;
}

Vector2 Vector2::operator -=(float& scolor)
{
	x -= scolor;
	y -= scolor;

	return *this;
}

Vector2 Vector2::operator *=(float& scolor)
{
	x *= scolor;
	y *= scolor;

	return *this;
}

Vector2 Vector2::operator /=(float& scolor)
{
	x /= scolor;
	y /= scolor;

	return *this;
}

Vector2 Vector2::operator +=(double& scolor)
{
	x += scolor;
	y += scolor;

	return *this;
}

Vector2 Vector2::operator -=(double& scolor)
{
	x -= scolor;
	y -= scolor;

	return *this;
}

Vector2 Vector2::operator *=(double& scolor)
{
	x *= scolor;
	y *= scolor;

	return *this;
}

Vector2 Vector2::operator /=(double& scolor)
{
	x /= scolor;
	y /= scolor;

	return *this;
}