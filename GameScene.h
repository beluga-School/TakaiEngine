#pragma once
#include "Input.h"

#include "Obj.h"

class GameScene
{
public:
	
	void Initialize();
	void Update();
	void Draw();
	void End();
private:

	Input* input = Input::GetInstance();

	Model triangleM = Model();
	Model skydome = Model();

	std::unique_ptr<Texture> zawa = std::make_unique<Texture>();
	std::unique_ptr<Texture> slime = std::make_unique<Texture>();
	std::unique_ptr<Texture> pizza = std::make_unique<Texture>();
	std::unique_ptr<Texture> tyusiten = std::make_unique<Texture>();
	std::unique_ptr<Texture> sinomiya = std::make_unique<Texture>();
	std::unique_ptr<Texture> white = std::make_unique<Texture>();
};

