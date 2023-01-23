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

	Vector4(float x, float y, float z, float w) {
		vec = { x,y,z };
		this->w = w;
	};
};