#pragma once
#include "Vector2.h"

class Wax
{
public:
	Wax(Vector2 pos);

	void Update();
	void Draw();

	Vector2 position;
	Vector2 velocity;

	bool isDead = false;

private:
	Vector2 speed = { 5,5 };
};

