#include "Vector2.h"
#include <math.h>

//Vec2
Vector2::Vector2(const float& x, const float& y) : x(x), y(y)
{
}

Vector2::Vector2() : x(0), y(0)
{
}

float Vector2::GetLength() const
{
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

//自身を書き換えないやつら
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

Vector2 Vector2::operator *(const int32_t& other) const
{
	Vector2 v;
	v.x = x * other;
	v.y = y * other;
	return v;
}

Vector2 Vector2::operator /(const int32_t& other) const
{
	Vector2 v;
	v.x = x / other;
	v.y = y / other;
	return v;
}

Vector2 Vector2::operator *(const float& other) const
{
	Vector2 v;
	v.x = x * other;
	v.y = y * other;
	return v;
}

Vector2 Vector2::operator /(const float& other) const
{
	Vector2 v;
	v.x = x / other;
	v.y = y / other;
	return v;
}

//自身を書き換えるやつら
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

Vector2 Vector2::operator +=(const int32_t& scolor)
{
	x += scolor;
	y += scolor;

	return *this;
}

Vector2 Vector2::operator -=(const int32_t& scolor)
{
	x -= scolor;
	y -= scolor;

	return *this;
}

Vector2 Vector2::operator *=(const int32_t& scolor)
{
	x *= scolor;
	y *= scolor;

	return *this;
}

Vector2 Vector2::operator /=(const int32_t& scolor)
{
	x /= scolor;
	y /= scolor;

	return *this;
}

Vector2 Vector2::operator +=(const float& scolor)
{
	x += scolor;
	y += scolor;

	return *this;
}

Vector2 Vector2::operator -=(const float& scolor)
{
	x -= scolor;
	y -= scolor;

	return *this;
}

Vector2 Vector2::operator *=(const float& scolor)
{
	x *= scolor;
	y *= scolor;

	return *this;
}

Vector2 Vector2::operator /=(const float& scolor)
{
	x /= scolor;
	y /= scolor;

	return *this;
}