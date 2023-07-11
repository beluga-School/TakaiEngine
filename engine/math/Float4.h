#pragma once
#include "Vector3.h"

struct Float4
{
	float x;
	float y;
	float z;
	float w;

	Float4() {
		x = 0;
		y = 0;
		z = 0;
		w = 0;
	};

	Float4(const float& x_,const float& y_,const float& z_,const float& w_) {
		x = x_;
		y = y_;
		z = z_;
		w = w_;
	};
};