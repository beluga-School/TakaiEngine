#pragma once
#include "Entity.h"

class CameraHitCheck : public Entity
{
public:
	CameraHitCheck() : Entity()
	{
		SetTag(TagTable::CameraHitCheck);
	}

	void Initialize()override;
	void Update()override;
	void Draw()override;
};

