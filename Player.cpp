#include "Player.h"
#include "Input.h"
#include "TimeManager.h"
#include "Stage.h"
#include "ImguiManager.h"
#include "MathF.h"
#include "PlayerCamera.h"
#include "EnemyManager.h"
#include "ClearDrawScreen.h"
#include "ObjParticle.h"

using namespace Input;

void Player::Initialize()
{
	Obj3d::Initialize();
	SetModel(ModelManager::GetModel("beetle"));

	colDrawer.Initialize();
	colDrawer.SetModel(ModelManager::GetModel("ICOSphere"));
	colDrawer.SetTexture(TextureManager::GetTexture("white"));

	SetOutLineState({ 0.1f,0.1f,0.1f,1.0f }, 0.05f);
}

void Player::Update()
{
	//ダメージ処理
	if (Input::Keyboard::TriggerKey(DIK_T))
	{
		hp.current -= 1;
	}

	if (hp.DecreaseTrigger())
	{
		for (int i = 0; i < 3; i++)
		{
			ParticleManager::GetInstance()->CreateCubeParticle(position,
				{ 3,3,3 }, 10, { 1,0,0,1 });
		}
	}
	if (hp.current < 0)
	{
		hp.current = 8;
		Stage::Get()->ChangeLevel(*LevelLoader::Get()->GetData("stage_graveyard"));
	}

	if (PlayerCamera::Get()->GetCamMode() == PlayerCamera::CamMode::Normal)
	{
		starState = StarState::None;
	}
	if (PlayerCamera::Get()->GetCamMode() == PlayerCamera::CamMode::StarGet)
	{
		starState = StarState::Get;
	}

	//移動地を初期化
	moveValue = { 0,0,0 };

	//ベクトルを保存
	mCenterVec = matWorld.ExtractAxisZ();
	mSideVec = matWorld.ExtractAxisX();

	Attack();

	//横移動更新
	if (attackState != AttackState::Attacking)
	{
		SideMoveUpdate();
	}
	
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

	UpdateY();

	//当たり判定更新
	ColUpdate();

	//本加算
	//position.y = preMove.y;
	position += moveValue;

	//当たり判定
	///--敵当たり判定
	playerCol.center = position;
	playerCol.radius = (scale.x + scale.y + scale.z) / 3.f;

	colDrawer.position = position;
	colDrawer.scale = scale;


	//回転更新
	//原神みたいな止まってるときは前向ける設計にしたいな〜〜
	if (attackState != AttackState::Attacking)
	{
		//スター取得中は正面を向けるため回転の更新をストップ(中に書いてある)
		RotaUpdate();
	}

	//更新
	Obj3d::Update(*Camera::sCamera);

	colDrawer.Update(*Camera::sCamera);
}

void Player::Draw()
{
	BasicObjectPreDraw(PipelineManager::GetPipeLine("OutLine"), false);
	Obj3d::DrawOutLine();
	
	/*BasicObjectPreDraw(PipelineManager::GetPipeLine("WireFrame"));
	colDrawer.Draw();*/

	BasicObjectPreDraw(PipelineManager::GetPipeLine("Toon"));
	Obj3d::DrawMaterial();
}

void Player::Reset()
{
	gravity = 0;
	hitListY.clear();
}

void Player::Attack()
{
	//攻撃更新
	attackingTimer.Update();
	attackCoolTimer.Update();
	attackingMotionTimer.Update();

	//入力
	switch (attackState)
	{
	case Player::AttackState::None:
		//攻撃していない状態で入力があったら
		//if (Mouse::Triggered(Click::LEFT))
		if(0)
		{
			//攻撃状態に遷移
			attackState = AttackState::Attacking;
			attackingTimer.Start();

			//ここで正面ベクトルを保存
			mRolingVec = mCenterVec;

			//ローリング数が適用されるように最大時間を割る
			attackingMotionTimer.mMaxTime = attackingTimer.mMaxTime;
			//attackingMotionTimer.mMaxTime = attackingTimer.mMaxTime;
		}
		break;
	case Player::AttackState::Attacking:

		if (attackingMotionTimer.GetRun() == false)
		{
			attackingMotionTimer.Start();
		}

		//正面ベクトルの方向に進める
		moveValue += mRolingVec * mRolingSpeed * TimeManager::deltaTime;
		rotation.x = TEasing::InQuad(-MathF::PIf * 2.f, MathF::PIf * 2.f, attackingMotionTimer.GetTimeRate());

		if (attackingTimer.GetEnd())
		{
			attackState = AttackState::CoolTime;
			attackingTimer.Reset();
			attackCoolTimer.Start();

			rotation.x = 0;
		}
		break;
	case Player::AttackState::CoolTime:
		if (attackCoolTimer.GetEnd())
		{
			attackState = AttackState::None;
			attackCoolTimer.Reset();
		}
		break;
	}
}

void Player::SideMoveUpdate()
{
	//星取得中なら移動できなくする
	if (starState != StarState::None)return;

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
	pCol.scale = scale;

	//pCol.position.y = preMove.y;

	pCol.position += moveValue;

	//for (auto& bColTemp : Stage::Get()->mColCubes)

	CreateCol(pCol.position, pCol.scale);
	for (auto& bColTemp : Stage::Get()->mColObj3ds)
	{
		Block* block = static_cast<Block*>(&bColTemp);
		CollideManager::Get()->CheckCollide(this, block);
	}

	//GroundCol();

	for (auto& bColevent : Stage::Get()->mEventObjects)
	{
		Cube eCol;
		eCol.position = bColevent->position;
		
		//なんか判定が小さかったので2倍に そしたらぴったりだったので、どっかで半分に
		//する処理が挟まってる
		eCol.scale = bColevent->scale * 2;
		
		if (Collsions::CubeCollision(eCol, pCol))
		{
			bColevent->HitEffect();

			break;
		}
	}

	for (auto& star : StarManager::Get()->mStars)
	{
		Cube eCol;
		eCol.position = star->position;

		//なんか判定が小さかったので2倍に そしたらぴったりだったので、どっかで半分に
		//する処理が挟まってる
		eCol.scale = star->scale * 2;

		if (Collsions::CubeCollision(eCol, pCol))
		{
			star->HitEffect();

			break;
		}
	}

	for (auto& goal : Stage::Get()->mGoals)
	{
		Cube goalCol;
		goalCol.position = goal->position + goal->goalBlock.position;

		//なんか判定が小さかったので2倍に そしたらぴったりだったので、どっかで半分に
		//する処理が挟まってる
		goalCol.scale = goal->goalBlock.scale * 2;

		if (Collsions::CubeCollision(goalCol, pCol))
		{
			goal->HitEffect();

			break;
		}
	}

	for (auto& enemy : EnemyManager::Get()->enemyList)
	{
		Cube enemyCol;
		enemyCol.position = enemy->position;
		enemyCol.scale = enemy->scale;
		
		if (Collsions::CubeCollision(enemyCol, pCol))
		{
			enemy->HitEffect();
		}
		
		if (Collsions::SphereCollsion(playerCol, enemy->sphereCol))
		{
			enemy->Encount();
		}
	}
}

void Player::RotaUpdate()
{
	//星取得中なら移動できなくする
	if (starState != StarState::None)return;

	//回転させる処理
	rotation.y = PlayerCamera::Get()->mHorizontalRad;
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
