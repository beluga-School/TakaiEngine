#include "Color.h"

Color::Color()
{
	f4.vec.x = 1.f;
	f4.vec.y = 1.f;
	f4.vec.z = 1.f;
	f4.w = 1.f;
}

Color::Color(const float& r, const float& g, const float& b, const float& a)
{
	f4.vec.x = r;
	f4.vec.y = g;
	f4.vec.z = b;
	f4.w = a;
}
