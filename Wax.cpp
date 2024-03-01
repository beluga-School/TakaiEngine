#include "Wax.h"
#include "InstantDrawer.h"
#include "MathF.h"
#include "TimeManager.h"

Wax::Wax(Vector2 pos)
{
	position = pos;
	velocity = { 0,0 };
}

void Wax::Update()
{
	position += velocity * speed;

	speed.y -= 5 * TimeManager::deltaTime;

	speed.y = Util::Clamp(speed.y, -5.f, 5.f);

	if (Util::OutOfScreen(position))
	{
		isDead = true;
	}
}

void Wax::Draw()
{
	InstantDrawer::DrawBox(position.x, position.y, 32, 32, { 1,1,1,1 });
}
