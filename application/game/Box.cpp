#include "Box.h"
#include "CollideManager.h"

uint32_t IDdCube::sMasterIDCount = 0;

void Box::CreateCol(const Vector3& pos_, const Vector3& scale_)
{
	cubecol.position = pos_;
	cubecol.scale = scale_;
}

void Box::ColDrawerUpdate(const Vector3& pos, const Vector3& scale_)
{
	position = pos;
	scale = scale_;

	Update(*Camera::sCamera);
}

uint32_t IDdCube::GetID()const
{
	return masterID;
}

void IDdCube::ResetID()
{
	sMasterIDCount = 0;
}