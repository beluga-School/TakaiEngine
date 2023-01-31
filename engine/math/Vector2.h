#pragma once
class Vector2
{
public:
	float x;
	float y;

	Vector2(float x, float y);
	Vector2();

	/// <summary>
	/// 長さを求める関数
	/// </summary>
	/// <returns>長さ</returns>
	/// このconstは、関数の内部で数値を書き換えないという意味を表す
	/// constがあることで、機能的にも書き換えることはできなくなる
	float GetLength() const;

	/// <summary>
	/// 自身を正規化する関数
	/// </summary>
	void Normalized();

	/// <summary>
	/// 正規化する関数
	/// </summary>
	/// <returns>Vec2の正規化された値</returns>
	Vector2 GetNormalized() const;

	/// <summary>
	/// 外積求める関数
	/// </summary>
	/// <param name="other"></param>
	/// <returns></returns>
	constexpr float Cross(const Vector2& other) const;

	/// <summary>
	/// 内積求める関数
	/// </summary>
	/// <param name="other"></param>
	/// <returns></returns>
	constexpr float Dot(const Vector2& other) const;

	//自身を書き換えないやつら
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

	//自身を書き換えるやつら
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