#pragma once
#include "Input.h"
#include "Pipeline.h"
#include "Sprite.h"
#include "ClearDrawScreen.h"
#include "ViewProjection.h"
#include "Billboard.h"
#include "Player.h"
#include "Stage.h"
#include "GroundEnemy.h"
#include "GoalObject.h"
#include "AirEnemy.h"

enum class Scene
{
	Title,
	Game,
	Clear,
	GameOver,
};

class Game
{
public:
	void Initialize();
	void Update();
	void Draw();
	void End();

	//カメラの更新(プレイヤーに追従)
	void CameraUpdate();

	//シーンチェンジの暗転
	void SceneChange(Scene scene);
	void SceneChangeUpdate();

	void SetAirEnemy(Vector3 position);
	void SetGroundEnemy(Vector3 position);
public:

	Input* input = Input::GetInstance();
	Sprite sceneChangeBlockOut;
	PipelineSet object3dPipelineSet;
	PipelineSet geometryObjectPipelineSet;
	
	Camera* camera = Camera::camera;

	Obj3d skydome;

	Stage stage;
	Player player;
	std::list<GroundEnemy> gEnemyList;

	std::list<AirEnemy> airEnemyList;

	Sprite goalSprite;
	Sprite TitleSprite;
	Sprite startSprite;
	Sprite spaceSprite;
	Sprite dashIconSprite;
	Sprite dashCoolSprite;

	Scene scene = Scene::Title;
	Scene nextScene = scene;

	GoalObject goal;
	
	bool colflag = false;
};

