#pragma once
#include "Input.h"
#include "Pipeline.h"
#include "Sprite.h"
#include "ClearDrawScreen.h"
#include "Particle.h"
#include "ViewProjection.h"
#include "Billboard.h"
#include "Player.h"
#include "Stage.h"
#include "GroundEnemy.h"

class GameScene
{
public:
	void Initialize();
	void Update();
	void Draw();
	void End();

	//カメラの更新(プレイヤーに追従)
	void CameraUpdate();
public:

	Input* input = Input::GetInstance();
	Sprite pizzaSprite;
	Sprite slimeSprite;
	PipelineSet object3dPipelineSet;
	PipelineSet geometryObjectPipelineSet;
	
	Camera* camera = Camera::camera;
	//Billboard billboard = Billboard(camera, false);
	//ParticleManager* particleManager = ParticleManager::Getinstance();
	Obj3d skydome;

	Stage stage;
	Player daruma;
	GroundEnemy gEnemy;

	bool colflag = false;
};

