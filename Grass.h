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
	//Obj3d poligon1;
	//Obj3d poligon2;

	TEasing::easeTimer timer;
};

