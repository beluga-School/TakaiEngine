#pragma once
#include "Vector3.h"

struct Vector4
{
	Vector3 vec;
	float w;

	Vector4() {
		vec = { 0,0,0 };
		w = 0;
	};

	Vector4(const float& x,const float& y,const float& z,const float& w) {
		vec = { x,y,z };
		this->w = w;
	};
};