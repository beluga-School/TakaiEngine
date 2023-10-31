#pragma once
#include <Vector3.h>
#include <Color.h>

class Line
{
public:
	Vector3 start{};
	Vector3 end{};

	Color color{};

	float thickness = 0;

	void Draw();
};

