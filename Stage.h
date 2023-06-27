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

	//モデルの配列
	std::list<Obj3d> mObj3ds;

	//当たり判定配列
	std::list<Cube> mColCubes;

private:
	Stage(){};
	~Stage(){};
};

