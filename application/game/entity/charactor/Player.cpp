#include "Player.h"
#include "Input.h"
#include "TimeManager.h"
#include "StageChanger.h"
#include "ImguiManager.h"
#include "MathF.h"
#include "PlayerCamera.h"
#include "EnemyManager.h"
#include "ClearDrawScreen.h"
#include "ObjParticle.h"
#include "StageTitleUI.h"
#include "EventManager.h"
#include "GameUIManager.h"

using namespace Input;

void Player::LoadResource()
{
	
}

void Player::Initialize()
{
	Obj3d::Initialize();
	SetModel(ModelManager::GetModel("beetle"));

	colDrawer.Initialize();
	colDrawer.SetModel(ModelManager::GetModel("ICOSphere"));
	colDrawer.SetTexture(TextureManager::GetTexture("white"));

	SetOutLineState({ 0.1f,0.1f,0.1f,1.0f }, 0.05f);

	hpGauge.Initialize();
}

void Player::Update()
{
	apparranceTimer.Update();
	if (playerState == PlayerState::Apparrance)
	{
		Vector3 endpos = saveDokanPos + Vector3(0, saveDokanScale.y / 2.f,0);
		position = TEasing::OutQuad(saveDokanPos, endpos,apparranceTimer.GetTimeRate());

		if (apparranceTimer.GetEnd())
		{
			ChangeMode(PlayerState::Normal);
			
			//ここら辺システム側の処理だから、別の場所に移したい
			GameUIManager::Get()->Move(UIMove::START,"StageTitle");
		}
	}

	//移動地を初期化
	moveValue = { 0,0,0 };

	//ベクトルを保存
	mCenterVec = matWorld.ExtractAxisZ();
	mSideVec = matWorld.ExtractAxisX();

	switch (playerState)
	{
	case Player::PlayerState::Normal:

		SideMoveUpdate();

		//縦移動更新(重力落下含む)
		//ジャンプしていない状態で入力があったらジャンプ
		if (jumpState == JumpState::None)
		{
			if (Input::Keyboard::TriggerKey(DIK_SPACE) ||
				Pad::TriggerPadButton(PadButton::A))
			{
				Jump();
			}
		}

		if (Input::Keyboard::TriggerKey(DIK_1))
		{
			ChangeMode(PlayerState::Debug);
		}

		break;
	case Player::PlayerState::Apparrance:

		break;
	case Player::PlayerState::Debug:

		SideMoveUpdate();

		Fly();

		//ダメージ処理テスト
		if (Input::Keyboard::TriggerKey(DIK_T))
		{
			mutekiTimer.Reset();
			DamageEffect(1);
		}
		if (Input::Keyboard::TriggerKey(DIK_G))
		{
			hp.mCurrent += 1;
		}

		if (Input::Keyboard::TriggerKey(DIK_1))
		{
			ChangeMode(PlayerState::Normal);
		}

		//入力でリロード
		if (Input::Keyboard::TriggerKey(DIK_R))
		{
			StageChanger::Get()->Reload();
		}

		break;
	}

	//Player特有の当たり判定更新(CollideManagerに移す)
	//ここ置き換えるまでは今日やる

	//Mob側の更新
	Mob::CollsionUpdate();

	box.ColDrawerUpdate(position, box.scale);
	box.CreateCol(position, box.scale);

	ColUpdate();

	//当たり判定
	///--敵当たり判定
	mEncountCol.center = position;
	mEncountCol.radius = (scale.x + scale.y + scale.z) / 3.f;

	colDrawer.position = position;
	colDrawer.scale = scale;

	//回転更新
	//スター取得中は正面を向けるため回転の更新をストップ(中に書いてある)
	RotaUpdate();

	//更新
	Obj3d::Update(*Camera::sCamera);

	colDrawer.Update(*Camera::sCamera);

	//当たり判定後、ステータスの更新
	DamageUpdate();

	hpGauge.Update();

	constBufferPlayerPos.mConstBufferData->pos = Player::Get()->position;
}

void Player::Draw()
{
	BasicObjectPreDraw("OutLine", false);
	Obj3d::DrawOutLine();
	
	BasicObjectPreDraw("Toon");
	Obj3d::DrawMaterial();
}

void Player::DrawCollider()
{
	BasicObjectPreDraw("Toon");
	box.Draw();
}

void Player::DrawUI()
{
	hpGauge.Draw();
}

void Player::Reset()
{
	gravity = 0;
	hitListUp.clear();
	hitListDown.clear();
	hitListBack.clear();
	hitListCenter.clear();
	hitListRight.clear();
	hitListLeft.clear();
}

void Player::SideMoveUpdate()
{
	accelerationTimer.Update();
	decelerationTimer.Update();

	//入力があったら、タイマーが増えていき、
	//入力がなかったら、タイマーが減っていく

	//移動量を取得、加算
	if ((Pad::GetLStickMove().GetLength() != 0 || 
		Pad::GetRStickMove().GetLength() != 0) ||
		(Input::Keyboard::PushKey(DIK_W) || 
		Input::Keyboard::PushKey(DIK_S) || 
		Input::Keyboard::PushKey(DIK_A) || 
		Input::Keyboard::PushKey(DIK_D)))
	{
		if (accelerationTimer.GetStarted() == false)
		{
			accelerationTimer.Start();
		}
		decelerationTimer.Reset();
	
		//加速を速度に反映
		mSpeed = TEasing::InQuad(0.0f, MAX_SPEED, accelerationTimer.GetTimeRate());
	
		//前回の移動方向を記録(キーボードのみ)
		oldMoveVec += mCenterVec * static_cast<float>((Input::Keyboard::PushKey(DIK_W) - Input::Keyboard::PushKey(DIK_S)));
		oldMoveVec += mSideVec * static_cast<float>((Input::Keyboard::PushKey(DIK_D) - Input::Keyboard::PushKey(DIK_A)));

		oldMoveVec.normalize();
	}
	else if(oldMoveVec.length() != 0)//前フレームでキーボード入力があったら(パッドなら減速なし)
	{
		if (decelerationTimer.GetStarted() == false)
		{
			decelerationTimer.Start();
			//速度を保存
			mSaveSpeed = mSpeed;
		}
		accelerationTimer.Reset();

		//加速を速度に反映
		mSpeed = TEasing::InQuad(mSaveSpeed,0.0f, decelerationTimer.GetTimeRate());
	}

	//減速中の移動
	if (decelerationTimer.GetRun())
	{
		moveValue += oldMoveVec * mSpeed * TimeManager::deltaTime;
	}

	if (Input::Pad::CheckConnectPad())
	{
		moveValue += mCenterVec * Input::Pad::GetLStickMove().y * mSpeed * TimeManager::deltaTime;
		moveValue += mSideVec * Input::Pad::GetLStickMove().x * mSpeed * TimeManager::deltaTime;
	}

	if (Input::Keyboard::PushKey(DIK_W))
	{
		moveValue += mCenterVec * mSpeed * TimeManager::deltaTime;
	}
	if (Input::Keyboard::PushKey(DIK_S))
	{
		moveValue -= mCenterVec * mSpeed * TimeManager::deltaTime;
	}
	if (Input::Keyboard::PushKey(DIK_D))
	{
		moveValue += mSideVec * mSpeed * TimeManager::deltaTime;
	}
	if (Input::Keyboard::PushKey(DIK_A))
	{
		moveValue -= mSideVec * mSpeed * TimeManager::deltaTime;
	}

	//あとで移動速度の加算に上限付ける
}

void Player::ColUpdate()
{
	///--地面当たり判定
	Cube pCol;
	pCol.position = position;
	//無理やりちょっと大きく
	pCol.scale = scale;

	pCol.position += moveValue;

	for (auto& goal : StageChanger::Get()->mGoals)
	{
		Cube goalCol;
		goalCol.position = goal->position + goal->goalBlock.position;

		//スケール参照なので、大きさが1固定になっちゃってる
		goalCol.scale = goal->goalBlock.scale * 2;

		if (Collsions::CubeCollision(goalCol, pCol))
		{
			goal->HitEffect();

			break;
		}
	}
}

void Player::RotaUpdate()
{
	//回転させる処理
	rotation.y = PlayerCamera::Get()->mHorizontalRad;
}

void Player::Fly()
{
	Vector3 upVec = {0,1,0};
	
	if (Input::Keyboard::PushKey(DIK_SPACE))
	{
		moveValue += upVec * mSpeed * TimeManager::deltaTime;
	}

	if (Input::Keyboard::PushKey(DIK_LSHIFT))
	{
		moveValue -= upVec * mSpeed * TimeManager::deltaTime;
	}
}

void Player::DamageUpdate()
{
	mutekiTimer.Update();
	flashTimer.Update();

	//無敵時間中は点滅させる
	if (mutekiTimer.GetRun())
	{
		if (flashTimer.GetRun() == false)
		{
			flashTimer.Start();
			mIsVisiable = !mIsVisiable;
		}
	}
	//無敵時間が終了したら点滅解除
	if (mutekiTimer.GetEnd())
	{
		if (mIsVisiable == false)
		{
			mIsVisiable = true;
		}
		mutekiTimer.Reset();
	}

	//外側でHPの最大値を超える処理を書いていたら丸める
	hp.mCurrent = Util::Clamp(hp.mCurrent, -1, MAX_HP);

	//ダメージを受けたとき
	if (hp.DecreaseTrigger())
	{
		//エフェクト出す
		for (int i = 0; i < 3; i++)
		{
			ParticleManager::GetInstance()->CreateCubeParticle(position,
				{ 3,3,3 }, 10, { 1,0,0,1 });
		}
	}
	//0になったら墓場へいく
	//いったんいらないので消す 今後別の形で再利用
	if (hp.mCurrent <= 0)
	{
		//hp.mCurrent = MAX_HP;
		//hpGauge.SetGaugeSize(hp.mCurrent, true);
		//StageChanger::Get()->ChangeLevel(*LevelLoader::Get()->GetData("stage_graveyard"));
	}

	//hpゲージの色を変える処理
	int32_t changeIndex = 0;
	
	if (hp.DecreaseTrigger())
	{
		changeIndex = (MAX_HP - hp.mCurrent) - 1;
		changeIndex = Util::Clamp(changeIndex, 0, MAX_HP);
		
		hpGauge.Addition(-1);
	}
	if (hp.IncreaseTrigger())
	{
		changeIndex = (MAX_HP - hp.mCurrent);
		changeIndex = Util::Clamp(changeIndex, 0, MAX_HP);

		hpGauge.Addition(1);
	}
}

bool Player::IsMove()
{
	return moveValue.x != 0 || moveValue.y != 0 || moveValue.z != 0;
}

void Player::DamageEffect(int32_t damage)
{
	//無敵時間中ならスキップ
	if (mutekiTimer.GetRun())return;

	//無敵時間開始
	mutekiTimer.Start();
	//ダメージ受ける
	hp.mCurrent -= damage;
}

void Player::ApparranceMove(const Vector3& dokanPos, const Vector3& dokanScale)
{
	ChangeMode(PlayerState::Apparrance);

	apparranceTimer.Start();
	saveDokanPos = dokanPos;
	saveDokanScale = dokanScale;
}

bool Player::GetApparanceEnd()
{
	return apparranceTimer.GetEnd();
}

void Player::HPOverFlow(int32_t value)
{
	hp.mCurrent = value;
	//HPのオーバーフロー処理
	if (hp.mCurrent > MAX_HP)
	{
		MAX_HP = hp.mCurrent;
		hpGauge.SetGaugeSize(hp.mCurrent, true);
	}
}

void Player::ChangeMode(const PlayerState& pState)
{
	playerState = pState;
	if (playerState == PlayerState::Normal)
	{
		SetNoCollsion(false);
		SetNoGravity(false);
		SetNoMove(false);
	}
	if (playerState == PlayerState::Apparrance)
	{
		SetNoCollsion(true);
		SetNoGravity(true);
		SetNoMove(true);
	}
	if (playerState == PlayerState::Debug)
	{
		SetNoCollsion(false);
		SetNoGravity(true);
		SetNoMove(false);
	}
}

void Player::Jump()
{
	//値を指定
	upJumpS = position.y;
	upJumpE = position.y + jumpPower;

	jumpManageTimer.Start();
	jumpState = JumpState::Up;

	//重力を無効化
	gravity = 0;
}

int32_t Player::GetNowHP()
{
	return hp.mCurrent;
}

