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
#include "Sound.h"
#include "LightGroup.h"

//enum class Scene
//{
//	Title,
//	Game,
//	Clear,
//	GameOver,
//};
const int max = 500;

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
	//void SceneChange(Scene scene);
	//void SceneChangeUpdate();

	void SetAirEnemy(Vector3 position, bool hanten = false);
	void SetGroundEnemy(Vector3 position, bool hanten = false);
private:

	void Reset();
	void DamageEffect();

	Input* input = Input::GetInstance();

	PipelineSet object3dPipelineSet;
	PipelineSet geometryObjectPipelineSet;
	
	Camera* camera = Camera::camera;

	Obj3d skydome;

	Stage stage;
	Player player;

	Obj3d firewispnorm;
	Obj3d firewispsmooth;
	Obj3d lightObj;

	std::list<GroundEnemy> gEnemyList;

	std::list<AirEnemy> airEnemyList;

	Sprite goalSprite;
	Sprite TitleSprite;
	Sprite startSprite;
	Sprite spaceSprite;
	Sprite redScreenSprite;
	Sprite setumeiSprite;
	Sprite setumei2Sprite;
	Sprite gameOverSprite;

	Obj3d cube[max];
	Vector3 cuberotaVec[max];

	SoundData push;
	SoundData goalSound;
	SoundData deadEnemy;

	float setumeiEndTime = 0;

	bool goalSoundFlag = false;

	//Scene scene = Scene::Game;
	//Scene nextScene = scene;
	
	bool colflag = false;

	LightGroup *lightGroup = nullptr;
};

