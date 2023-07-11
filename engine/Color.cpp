#include "Color.h"

Color::Color()
{
	f4.x = 1.f;
	f4.y = 1.f;
	f4.z = 1.f;
	f4.w = 1.f;
}

Color::Color(const float& r, const float& g, const float& b, const float& a)
{
	f4.x = r;
	f4.y = g;
	f4.z = b;
	f4.w = a;
}
