#include "Vector3.h"
#include <cmath>
#include "Util.h"

Vector3::Vector3(){
	x = 0;
	y = 0;
	z = 0;
}

Vector3::Vector3(const float& x, const float& y, const float& z) :x(x),y(y),z(z){

}

float Vector3::length() const {
	return sqrtf(x * x + y * y + z * z);
}

Vector3 Vector3::GetNormalize()const {
	Vector3 v;
	float len = length();
	if (len != 0)
	{
		return v = *this / len;
	}
	return v;
}

void Vector3::normalize()
{
	float leng = length();
	//0除算対策
	if (leng == 0)
	{
		x = 0;
		y = 0;
		z = 0;
		return;
	}

	x = x / leng;
	y = y / leng;
	z = z / leng;
}

void Vector3::cross(const Vector3& v)
{
	x = y * v.z - z * v.y;
	y = z * v.x - x * v.z;
	z = x * v.y - y * v.x;
}

Vector3 Vector3::GetCross(const Vector3& v)const
{
	Vector3 vec3;
	vec3.x = y * v.z - z * v.y;
	vec3.y = z * v.x - x * v.z;
	vec3.z = x * v.y - y * v.x;

	return vec3;
}

Vector3 Vector3::Spline(const std::vector<Vector3>& points, float t)
{
	//ひとつしか入っていないならそのまま返す
	if (points.size() <= 1) { return Vector3(points.front()); }
	t = Util::Clamp(t, 0.f, 1.f);

	float perSegment = 1.f / (points.size() - 1);
	int32_t currentIndex = (int32_t)(t / perSegment);

	if (t == 1.0f)
	{
		currentIndex -= 1;
	}

	t = t - currentIndex * perSegment;
	t = t / perSegment;

	currentIndex -= 1;

	const Vector3& p0 = points.at(currentIndex < 0 ? 0 : currentIndex);
	const Vector3& p1 = points.at(currentIndex + 1);
	const Vector3& p2 = points.at(currentIndex + 2);
	const Vector3& p3 = points.at(currentIndex + 3 >= points.size() ? points.size() - 1 : currentIndex + 3);

	return (p1 * 2 + (-p0 + p2) * t +
		(p0 * 2 - p1 * 5 + p2 * 4 - p3) * t * t +
		(-p0 + p1 * 3 - p2 * 3 + p3) * t * t * t) * 0.5f;
}

float Vector3::Radian(const Vector3& a) const
{
	Vector3 na = this->GetNormalize();
	Vector3 nb = a.GetNormalize();
	float dot = na.dot(nb); //長さ1同士のベクトルの内積はcosθとなる...
	return acosf(dot); //sqrtより重いまであるから気を付けた方が良い
}

float Vector3::dot(const Vector3& v) const
{
	return x * v.x + y * v.y + z * v.z;
}

Vector3 Vector3::operator+()const
{
	return *this;
}

Vector3 Vector3::operator-()const
{
	return Vector3(-this->x, -this->y, -this->z);
}

Vector3& Vector3::operator+=(const Vector3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;

	return *this;
}

Vector3& Vector3::operator-=(const Vector3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;

	return *this;
}

Vector3& Vector3::operator*=(const float s)
{
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

Vector3& Vector3::operator/=(const float s)
{
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

bool Vector3::operator==(const Vector3& v)const
{
	return x == v.x && y == v.y && z == v.z;
}

bool Vector3::operator!=(const Vector3& v)const
{
	return !(*this == v);
}

//Vector2 クラスに属さない関数群
//二項演算子
const Vector3 operator+(const Vector3& v1, const Vector3& v2)
{
	Vector3 temp(v1);
	return temp += v2;
}

const Vector3 operator-(const Vector3& v1, const Vector3& v2)
{
	Vector3 temp(v1);
	return temp -= v2;
}

const Vector3 operator*(const Vector3& v, const float s)
{
	Vector3 temp(v);
	return temp *= s;
}

const Vector3 operator*(const float s, const Vector3& v)
{
	return v * s;
}

const Vector3 operator/(const Vector3& v, const float s)
{
	Vector3 temp(v);
	return temp /= s;
}
