#pragma once
#include "Obj.h"
#include "Mob.h"
#include "Input.h"
#include "Stage.h"
#include "Vector2.h"
#include "Sound.h"

class Player : public Mob
{
public:
	Player() {
		Initialize();
	};

	void Respawn();

	void Initialize();

	//ステージとの当たり判定を行うため、ステージを引数で貰っている
	//ステージはどうせひとつなので、現在のステージをシングルトンで貰えるともっと綺麗かも
	//(nowCameraと同じ感覚で、nowStage的な)
	void Update(const Stage& stage);

	void Draw();

	void End();

	bool isWallGrap = false;
	bool isDash = false;
	float dashTimer = 0;
	Vector3 dashVec = { 0,0,0 };

	bool jumpSoundFlag = false;

	//Vector3 spawnPos = { 0,330,-560 };
	Vector3 spawnPos = { 0,0,0 };

	float horizontalRotation;
	float verticalRotation;

	POINT point;

	//マウスの感度
	float mouseSpd = 0.0025f;

	float dashCool = 0;

	bool oldGround = false;

	int32_t hp = 0;
	bool isDead = false;

	float mutekiTimer = 0;
	int32_t blinkTimer = 0;

	Vector2 max;

	void HitEffect();

	SoundData jump;
	SoundData dash;
	SoundData hit;

private:
	///---クラス内で使用する関数群

	//横移動
	void Move();

	//縦移動(ジャンプ)
	void Jump();

	///---シングルトン
	Input::Keyboard* input = Input::Keyboard::Get();
};