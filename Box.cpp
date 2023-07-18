#include "Box.h"
#include "CollideManager.h"

void Box::CreateCol()
{
	cubecol.position = position;
	cubecol.scale = scale;
}

void Box::CreateCol(const Vector3& pos_, const Vector3& scale_)
{
	cubecol.position = pos_;
	cubecol.scale = scale_;
}

void Box::Register()
{
	CollideManager::Get()->allCols.push_back(this);
}