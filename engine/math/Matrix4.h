#pragma once
#include "Vector3.h"
#include <stdint.h>

class Matrix4
{
public:
	static const int32_t ROW = 4;
	static const int32_t COL = 4;
	float m[ROW][COL];

	Matrix4();	//単位行列を代入

	Matrix4(
		const float& x00,const float& x01,const float& x02,const float& x03,
		const float& x10,const float& x11,const float& x12,const float& x13,
		const float& x20,const float& x21,const float& x22,const float& x23,
		const float& x30,const float& x31,const float& x32,const float& x33);

	static Matrix4 Identity();
	static Matrix4 scale(const Vector3& s);

	static Matrix4 rotateX(const float& angle);
	static Matrix4 rotateY(const float& angle);
	static Matrix4 rotateZ(const float& angle);

	static Matrix4 translate(const Vector3& t);
	static Vector3 transform(const Vector3& v, const Matrix4& m);

	//左手系のパースペクティブ射影変換行列を求める
	static Matrix4 PerspectiveFovLH(const float& fovAngleY,const float& nearZ,const float& farZ);
	//左手系のビュー変換行列を求める
	static Matrix4 LookAtLH(const Vector3& eye,const Vector3& target,const Vector3& up);

	//横ベクトルを抽出する
	Vector3 ExtractAxisX()const;
	//上方向ベクトルを抽出する
	Vector3 ExtractAxisY()const;
	//正面ベクトルを抽出する
	Vector3 ExtractAxisZ()const;

	static Matrix4 RotArbitrary(Vector3& axis,const float& rad);

	float* operator[](const int32_t& i);

	float& operator()(const int32_t& i,const int32_t& j);

	Matrix4& operator=(const Matrix4& a);
	//static Matrix4& operator*=(Matrix4& m1, Matrix4& m2);

	Matrix4 operator+(const Matrix4& a) const;
	Matrix4 operator-(const Matrix4& a) const;
	Matrix4 operator*(const Matrix4& a) const;

	//static Vector3 operator*(const Vector3& v, const Matrix4& m);

	//Matrix4& operator += (const Matrix4& a);
	//Matrix4& operator -= (const Matrix4& a);
	Matrix4& operator *= (const Matrix4& a);
};