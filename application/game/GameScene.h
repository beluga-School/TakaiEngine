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
#include <IScene.h>
#include "TEasing.h"

//enum class Scene
//{
//	Title,
//	Game,
//	Clear,
//	GameOver,
//};
const uint32_t MAX_CUBE = 500;

class Game : public IScene
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

	void SetAirEnemy(const Vector3& position, const bool& hanten = false);
	void SetGroundEnemy(const Vector3& position, const bool& hanten = false);
private:

	void Reset();
	void DamageEffect();

	Input::Keyboard* input = Input::Keyboard::Get();

	PipelineSet object3dPipelineSet;
	PipelineSet geometryObjectPipelineSet;
	
	Camera* camera = Camera::sCamera;

	Obj3d skydome;

	Stage stage;
	Player player;

	//当たり判定するためのやつら
	Obj3d firewispsmooth;
	Obj3d firewispCheckTriangle;
	Obj3d board;
	Obj3d lightObj;
	Obj3d triangle;

	std::list<GroundEnemy> gEnemyList;

	std::list<AirEnemy> airEnemyList;

	Sprite goalSprite;
	Sprite TitleSprite;
	Sprite startSprite;
	Sprite spaceSprite;
	Sprite redScreenSprite;
	Sprite slimeSprite;
	Sprite gameOverSprite;

	Obj3d cube[MAX_CUBE];
	Vector3 cuberotaVec[MAX_CUBE];

	SoundData push;
	SoundData goalSound;
	SoundData deadEnemy;

	float setumeiEndTime = 0;

	bool goalSoundFlag = false;

	//Scene scene = Scene::Game;
	//Scene nextScene = scene;
	
	bool colflag = false;

	TEasing::easeTimer sphereVal;
};

