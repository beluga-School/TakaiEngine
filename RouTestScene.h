#pragma once
#include "IScene.h"
#include "Vector2.h"
#include "Wax.h"
#include <list>

class RouTestScene : public IScene
{
public:
	void LoadResource()override;
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void End()override;

private:
	Vector2 center = { 0,0 };
	Vector2 mousePoint;

	std::list<Wax> waxs;
};

