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

void Player::LoadResource()
{
	starUI.LoadResource();
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

	starUI.Initialize("star2d");
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
			playerState = PlayerState::Normal;
			SetNoCollsion(false);
			SetNoGravity(false);
			SetNoMove(false);
		}
	}

	//ダメージ処理
	if (Input::Keyboard::TriggerKey(DIK_T))
	{
		mutekiTimer.Reset();
		DamageEffect(1);
	}
	if (Input::Keyboard::TriggerKey(DIK_G))
	{
		hp.mCurrent += 1;
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

	//Mob側の更新
	Mob::CollsionUpdate();

	//Player特有の当たり判定更新(CollideManagerに移す)
	//ここ置き換えるまでは今日やる
	ColUpdate();

	//本加算
	//動けないフラグ立ったら加算しない
	if (mNoMove == false)
	{
		position += moveValue;
	}

	//当たり判定
	///--敵当たり判定
	mEncountCol.center = position;
	mEncountCol.radius = (scale.x + scale.y + scale.z) / 3.f;

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

	//当たり判定後、ステータスの更新
	DamageUpdate();

	hpGauge.Update();

	StarUIUpdate();
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

void Player::DrawUI()
{
	hpGauge.Draw();
	starUI.Draw();
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

	box.ColDrawerUpdate(pCol.position, pCol.scale);
	box.CreateCol(pCol.position, pCol.scale);

	//ここら辺の処理を、全部CollideManagerに移す
	//今はここに置かないと横の判定が取れない仕組みになってるので、後でX方向もシステム化する
	for (auto& col : CollideManager::Get()->allCols)
	{
		if (col->CheckTag(TagTable::Block))
		{
			Block* block = static_cast<Block*>(col);
			CollideManager::Get()->CheckCollide(this, block);
		}
	}

	////ここより下二つは、Entityをポインタで保持するようにしてから修正する
	//for (auto& star : StarManager::Get()->mStars)
	//{
	//	Cube eCol;
	//	eCol.position = star->position;

	//	//なんか判定が小さかったので2倍に そしたらぴったりだったので、どっかで半分にする処理が挟まってる
	//	//判定用のスケールが使われてないのが原因
	//	eCol.scale = star->scale * 2;

	//	if (Collsions::CubeCollision(eCol, pCol))
	//	{
	//		star->HitEffect();

	//		break;
	//	}
	//}

	for (auto& goal : StageChanger::Get()->mGoals)
	{
		Cube goalCol;
		goalCol.position = goal->position + goal->goalBlock.position;

		//なんか判定が小さかったので2倍に そしたらぴったりだったので、どっかで半分にする処理が挟まってる
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
	if (hp.mCurrent <= 0)
	{
		hp.mCurrent = MAX_HP;
		hpGauge.SetGaugeSize(hp.mCurrent, true);
		StageChanger::Get()->ChangeLevel(*LevelLoader::Get()->GetData("stage_graveyard"));
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

void Player::StarUIUpdate()
{
	for (auto& obj : StageChanger::Get()->mEventObjects)
	{
		if (!obj->CheckTag(TagTable::Star))continue;

		Star* star = static_cast<Star*>(obj.get());
		if (star->GetState() == Star::StarState::CountUp)
		{
			starCorrectNum.mCurrent += 1;
			starUI.GetMoveStart(starCorrectNum.mCurrent);
			star->StateEnd();
			for (auto& star : StageChanger::Get()->mTempStarSaves)
			{
				StageChanger::Get()->CorrectedRevise(
					LevelLoader::Get()->GetData(StageChanger::Get()->GetNowStageHandle())->mStageNum,
					star->id,
					(int32_t)StageChanger::Get()->mTempStarSaves.size()
				);
			}
		}
		//スター取得中なら
		if (star->GetState() == Star::StarState::jumpUp ||
			star->GetState() == Star::StarState::Inhole)
		{
			//出現状態で固定する
			starUI.AppLock();
		}
	}

	UIDelayTimer.Update();
	if (IsMove())
	{
		starUI.DisAppearance(0.2f);
		UIDelayTimer.Reset();
	}
	else
	{
		if (UIDelayTimer.GetStarted() == false)
		{
			UIDelayTimer.Start();
		}
		if (UIDelayTimer.GetEnd())
		{
			starUI.Appearance(0.5f);
		}
	}

	starUI.Update();
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
	playerState = PlayerState::Apparrance;
	SetNoCollsion(true);
	SetNoGravity(true);
	SetNoMove(true);

	apparranceTimer.Start();
	saveDokanPos = dokanPos;
	saveDokanScale = dokanScale;
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

