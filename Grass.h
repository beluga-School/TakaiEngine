#pragma once
#include "Entity.h"
#include "TEasing.h"

class Grass : public Entity
{
public:
	Grass() : Entity()
	{
		
	}

	static void LoadResource();

	void Initialize()override;
	void Update()override;
	void Draw()override;

private:
	TEasing::easeTimer timer = 0.4f;
	int32_t bounceCount = 0;
};

