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
	SetModel(ModelManager::GetModel("Cube"));

	colDrawer.Initialize();
	colDrawer.SetModel(ModelManager::GetModel("ICOSphere"));
	colDrawer.SetTexture(TextureManager::GetTexture("white"));

	drawerObject.Initialize();
	drawerObject.SetModel(ModelManager::GetModel("beetle"));

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

	Vector3 playerBack{};
	Vector3 playerFeetPos{};
	
	switch (playerState)
	{
	case Player::PlayerState::Normal:

		jumpCount = 0;
		mSpeed = NORMAL_SPEED;
		MoveUpdate();

		if (Input::Keyboard::TriggerKey(DIK_1))
		{
			ChangeMode(PlayerState::Debug);
		}
		if (Input::Keyboard::PushKey(DIK_LSHIFT))
		{
			ChangeMode(PlayerState::Dash);
		}

		break;
	case Player::PlayerState::Dash:
		mSpeed = DASH_SPEED;
		MoveUpdate();

		//プレイヤーの少し後ろの足元座標を算出
		playerBack =  -matWorld.ExtractAxisZ() * (scale.z / 2);
		playerFeetPos = Vector3( position.x,position.y - (scale.y / 2),position.z ) + playerBack;

		//パーティクル配置
		ParticleManager::Get()->CreateCubeParticle(
			playerFeetPos,
			{0.5f ,0.5f ,0.5f}, 
			1.0f, 
			{1.0f,1.0f,1.0f,1.0f}
		);

		if (!Input::Keyboard::PushKey(DIK_LSHIFT))
		{
			ChangeMode(PlayerState::Normal);
		}

		break;
	case Player::PlayerState::Apparrance:

		break;
	case Player::PlayerState::InDokan:

		break;
	case Player::PlayerState::Debug:

		MoveUpdate();

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

	//Mob側の更新
	Mob::CollsionUpdate();

	box.ColDrawerUpdate(position, box.scale);
	box.CreateCol(position, box.scale, rotation);

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

	//描画用オブジェクトの更新
	drawerObject.position = position;
	drawerObject.scale = scale;
	drawerObject.Update(*Camera::sCamera);
}

void Player::Draw()
{
	BasicObjectPreDraw("OutLine", false);
	drawerObject.DrawOutLine();
	
	BasicObjectPreDraw("Toon");
	drawerObject.DrawMaterial();
	//Obj3d::DrawMaterial();
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

void Player::MoveUpdate()
{
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

	if (Input::Pad::CheckConnectPad())
	{
		moveValue += mCenterVec * Input::Pad::GetLStickMove().y * mSpeed * TimeManager::deltaTime;
		moveValue += mSideVec * Input::Pad::GetLStickMove().x * mSpeed * TimeManager::deltaTime;
	}

	if (Input::Keyboard::PushKey(DIK_W) || 
		Input::Keyboard::PushKey(DIK_S) ||
		Input::Keyboard::PushKey(DIK_D) ||
		Input::Keyboard::PushKey(DIK_A))
	{
		moveValue += mCenterVec * mSpeed * TimeManager::deltaTime;
	}
}

void Player::ColUpdate()
{
	///--地面当たり判定
	Cube pCol;
	pCol.position = position;
	//無理やりちょっと大きく
	pCol.scale = scale;

	pCol.position += moveValue;

	for (auto& obj : StageChanger::Get()->mEntitys)
	{
		Goal* goal = dynamic_cast<Goal*>(obj.get());
		if (goal != nullptr)
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
}

float RotaComparison(float now, float target)
{
	float one = target - now;
	
	float invTarget = Util::Abs(target - 360);
	
	float two = now + invTarget;

	return one > two;
}

void Player::RotaUpdate()
{
	//回転させる最終位置
	//上下の入力があるか
	if (Input::Keyboard::PushKey(DIK_W) || 
		Input::Keyboard::PushKey(DIK_S))
	{
		if (Input::Keyboard::PushKey(DIK_W)) {
			targetRota = 0;
			if (Input::Keyboard::PushKey(DIK_D)) {
				targetRota = 4;
			}
			if (Input::Keyboard::PushKey(DIK_A)) {
				targetRota = 315;
			}
		}
		if (Input::Keyboard::PushKey(DIK_S)) {
			targetRota = 180;
			if (Input::Keyboard::PushKey(DIK_D)) {
				targetRota = 135;
			}
			if (Input::Keyboard::PushKey(DIK_A)) {
				targetRota = 225;
			}
		}
	}
	//ないなら
	else
	{
		if (Input::Keyboard::PushKey(DIK_D))
		{
			targetRota = 90;
		}
		if (Input::Keyboard::PushKey(DIK_A))
		{
			targetRota = 270;
		}
	}

	rotaTimer.Update();

	//入力があった瞬間
	if (targetRota != oldTargetRota)
	{
		//回転タイマーを初期化し、現在位置を初めの回転に保存
		rotaTimer.Start();
		rotaStart = oldTargetRota;

		hoge = RotaComparison(rotaStart, targetRota);
		if (hoge)
		{
			targetRota -= 360;
		}
	}

	//初めの回転から最後の回転までをタイマーで動かす
	float check = MathF::AngleConvRad(TEasing::OutQuad(rotaStart, targetRota, rotaTimer.GetTimeRate()));
	rotation.y = PlayerCamera::Get()->mHorizontalRad + check;
	drawerObject.rotation.y = rotation.y;

	oldTargetRota = targetRota;

	//ジャンプ中なら
	if (IsJump())
	{
		if (jumpCount == 2)
		{
			plusRotaX -= 20.0f;
			//カメラ引きたい
		}
		else if (jumpCount >= 3)
		{
			plusRotaX += 40.0f;
		}
	}

	//着地してるなら取り消す
	if (!IsJump())
	{
		plusRotaX = 0;
		PlayerCamera::Get()->InitRadius();
	}

	drawerObject.rotation.x = MathF::AngleConvRad(plusRotaX);
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
			ParticleManager::Get()->CreateCubeParticle(position,
				{ 3,3,3 }, 10, { 1,0,0,1 });
		}
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

bool Player::IsJump()
{
	return (jumpState == JumpState::Up || jumpState == JumpState::Down);
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

Player::PlayerState Player::GetState()
{
	return playerState;
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
	if (playerState == PlayerState::Dash)
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
	if (playerState == PlayerState::InDokan)
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
	jumpCount++;

	jumpCount = Util::Clamp(jumpCount, 0, 3);

	if (jumpCount == 1)
	{
		jumpPower = 5.0f;
	}

	if (jumpCount == 2)
	{
		jumpPower = 7.5f;
		//カメラをちょい引く(もっと放してもいいかも)
		PlayerCamera::Get()->ChangeRadius(1.0f, 0.5f);
	}

	if (jumpCount == 3)
	{
		jumpPower = 10.0f;
		PlayerCamera::Get()->ChangeRadius(2.0f, 0.5f);
	}

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

