#pragma once
#include "Input.h"
#include "Pipeline.h"
#include "Sprite.h"
#include "ClearDrawScreen.h"
#include "Particle.h"
#include "ViewProjection.h"
#include "Billboard.h"
#include "Player.h"
#include "Block.h"

class GameScene
{
public:
	void Initialize();
	void Update();
	void Draw();
	void End();
public:

	Input* input = Input::GetInstance();
	Sprite pizzaSprite;
	Sprite slimeSprite;
	PipelineSet object3dPipelineSet;
	PipelineSet geometryObjectPipelineSet;
	Obj3d cubeObj;
	
	Camera* camera = Camera::camera;
	//Billboard billboard = Billboard(camera, false);
	//ParticleManager* particleManager = ParticleManager::Getinstance();
	Obj3d skydome;
	Obj3d board;

	Block stage;
	Player daruma;

	bool colflag = false;
};

