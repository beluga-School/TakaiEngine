#pragma once
#include <vector>

class Vector3
{
public:
	float x;	// x成分
	float y;	// y成分
	float z;	// z成分
public:
	Vector3();
	Vector3(const float& x,const float& y,const float& z);

	//メンバ関数

	/// <summary>
	/// 長さを返す
	/// </summary>
	float length() const;

	/// <summary>
	/// 正規化
	/// 結果をreturnし、本体を書き換えない 本体を書き換えたい場合はnormalizeを使用してください
	/// </summary>
	/// <param name="v"></param>
	/// <returns></returns>
	Vector3 GetNormalize()const;
	
	/// <summary>
	/// 正規化
	/// 本体を書き換える　変数に代入したい場合はGetNormalizeを使用してください 
	/// </summary>
	/// <param name="v"></param>
	/// <returns></returns>
	void normalize();
	
	/// <summary>
	/// 内積
	/// </summary>
	/// <param name="v"></param>
	/// <returns></returns>
	float dot(const Vector3& v)const;
	
	/// <summary>
	/// 外積
	/// 本体を書き換える　変数に代入したい場合はGetCrossを使用してください
	/// </summary>
	void cross(const Vector3& v);
	
	/// <summary>
	/// 外積
	/// 結果をreturnし、本体を書き換えない 本体を書き換えたい場合はcrossを使用してください
	/// </summary>
	Vector3 GetCross(const Vector3& v)const;

	/// <summary>
	/// Wrelf先生のやつ丸パクりSpline
	/// </summary>
	/// <param name="points"></param>
	/// <param name="t"></param>
	/// <returns></returns>
	static Vector3 Spline(const std::vector<Vector3>& points, float t);

	/// <summary>
	/// 2つのベクトルのなす角(Radian)を取得
	/// 激おもらしいので多様厳禁
	/// </summary>
	float Radian(const Vector3& a) const;

	Vector3 operator+()const;
	Vector3 operator-()const;

	Vector3& operator+=(const Vector3& v);
	Vector3& operator-=(const Vector3& v);
	Vector3& operator*=(const float s);
	Vector3& operator/=(const float s);
};

const Vector3 operator+(const Vector3& v1, const Vector3& v2);
const Vector3 operator-(const Vector3& v1, const Vector3& v2);
const Vector3 operator*(const Vector3& v1, const float s);
const Vector3 operator*(const float s, const Vector3& v1);
const Vector3 operator/(const Vector3& v1, float s);