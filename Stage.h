#pragma once
#include "LevelLoader.h"
#include "Obj.h"
#include "Collision.h"

class Stage
{
public:
	void Load(LevelData& data);

	void Update();
	void Draw();

	static Stage* Get()
	{
		static Stage instance;
		return &instance;
	}

	//ƒ‚ƒfƒ‹‚Ì”z—ñ
	std::list<Obj3d> mObj3ds;

	//“–‚½‚è”»’è”z—ñ
	std::list<Cube> mColCubes;

private:
	Stage(){};
	~Stage(){};
};

