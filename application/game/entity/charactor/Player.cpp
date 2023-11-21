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

	centerObject.Initialize();
	centerObject.SetModel(ModelManager::GetModel("beetle"));

	SetOutLineState({ 0.1f,0.1f,0.1f,1.0f }, 0.05f);

	hpGauge.Initialize();

	rotmode = RotMode::Quaternion;
	centerObject.rotmode = RotMode::Quaternion;

	quaternion = DirectionToDirection({0,0,0},{0,0,1});
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
	mCenterVec = centerObject.matWorld.ExtractAxisZ();
	mSideVec = centerObject.matWorld.ExtractAxisX();

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

		if (flyMode)
		{
			Fly();
		}

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
	centerObject.position = position;
	centerObject.scale = scale;
	centerObject.Update(*Camera::sCamera);
}

void Player::Draw()
{
	BasicObjectPreDraw("OutLine", false);
	Obj3d::DrawOutLine();
	
	BasicObjectPreDraw("Toon");
	Obj3d::DrawMaterial();
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
	wallKickTimer.Update();
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
	if (CanWallKick())
	{
		if (Input::Keyboard::TriggerKey(DIK_SPACE) ||
			Pad::TriggerPadButton(PadButton::A))
		{
			Jump();
			//反対方向に飛ぶ
			wallKickTimer.Start();
			wallKickVec = matWorld.ExtractAxisZ();
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

	//壁キックタイマーが動いてるなら壁キック入力があったときの逆進行方向を足し続ける
	if (wallKickTimer.GetRun())
	{
		moveValue = { 0,0,0 };
		//斜め上に飛ばしたい
		moveValue -= wallKickVec;
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

	//ゴールとの判定
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
	
	//rotation.x = MathF::AngleConvRad(jumpPlusRotaX);

	rotTime.Update();
	rotTime.mMaxTime = 0.5f;

	//⑴終点になるプレイヤーの向きたい方向をクオータニオンで取得する
	//入力方向をいい感じに取る
	Vector3 camCenterVec = PlayerCamera::Get()->matWorld.ExtractAxisZ();
	Vector3 camRightVec = PlayerCamera::Get()->matWorld.ExtractAxisX();
	camCenterVec.y = 0;
	camCenterVec.normalize();
	camRightVec.y = 0;
	camRightVec.normalize();

	Vector3 center = camCenterVec;
	Vector3 back = -camCenterVec;
	Vector3 right = camRightVec;
	Vector3 left = -camRightVec;

	if (Input::Keyboard::TriggerKey(DIK_W) ||
		Input::Keyboard::TriggerKey(DIK_S) ||
		Input::Keyboard::TriggerKey(DIK_D) ||
		Input::Keyboard::TriggerKey(DIK_A))
	{
		rotTime.Start();
		endRota = {0,0,0};
	}

	if (Input::Keyboard::PushKey(DIK_W))endRota += center;
	if (Input::Keyboard::PushKey(DIK_S))endRota += back;
	if (Input::Keyboard::PushKey(DIK_D))endRota += right;
	if (Input::Keyboard::PushKey(DIK_A))endRota += left;

	endQ = DirectionToDirection({ 0,0,1 }, endRota);

	//⑵始点になる今のプレイヤーの向きをクオータニオンで取得する
	Vector3 startRota;
	//回転を打ち消した始点のベクトルを作成
	startRota = centerObject.matWorld.ExtractAxisZ();
	//startRota = {0,0,1};
	startRota.y = 0;

	startQ = DirectionToDirection({ 0,0,1 }, startRota);

	//⑶(終点Q - 始点Q)を求める
	culQ = Slerp(startQ,endQ, rotTime.GetTimeRate());

	//ジャンプ中なら
	if (IsJump())
	{
		if (jumpCount == 2)
		{
			jumpPlusRotaX = MathF::AngleConvRad(-180.f);
		}
		else if (jumpCount >= 3)
		{
			jumpPlusRotaX = MathF::AngleConvRad(270.5f);
		}
	}

	if (wallKickTimer.GetEnd())
	{
		jumpPlusRotaX = MathF::AngleConvRad(90.f);
	}

	//着地してるなら取り消す
	if (!IsJump())
	{
		jumpPlusRotaX = 0;
		PlayerCamera::Get()->InitRadius();
	}

	//Quaternion rotX = MakeAxisAngle(matWorld.ExtractAxisX(), jumpPlusRotaX);

	//④現在のプレイヤーの向き(Q)に足しまくる
	//quaternion = rotX * culQ;
	quaternion = culQ;
	centerObject.quaternion = culQ;
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

bool Player::CheckContinuanceKick(IDdCube* check)
{
	//ひとつ前のセーブした壁とIDが同じならtrueを返す
	if (saveKickWall != nullptr)
	{
		if (saveKickWall->GetID() == check->GetID())
		{
			return true;
		}
	}
	//IDが不正な物でなければ
	if (saveKickWall == nullptr)
	{
 		saveKickWall = check;
	}
	return false;
}

bool Player::CanWallKick()
{
	//ジャンプ中でないなら不可
	if (jumpState != JumpState::Down)return false;

	//正面ベクトルにちょい足しした位置の四角と当たり判定して当たってるなら壁キックできる
	Cube cube;
	cube.position = position + (matWorld.ExtractAxisZ() * 5.0f);
	cube.scale = scale;
	cube.rotation = rotation;

	for (auto& hit : hitListCenter)
	{
		if (Collsions::CubeCollision(hit, cube))
		{
			//ひとつ前のセーブした壁とIDが同じなら失敗判定にする
			if (CheckContinuanceKick(&hit))
			{
				return false;
			}
			return true;
		}
	}
	for (auto& hit : hitListBack)
	{
		if (Collsions::CubeCollision(hit, cube))
		{
			//ひとつ前のセーブした壁とIDが同じなら失敗判定にする
			if (CheckContinuanceKick(&hit))
			{
				return false;
			}
			return true;
		}
	}
	for (auto& hit : hitListRight)
	{
		if (Collsions::CubeCollision(hit, cube))
		{
			//ひとつ前のセーブした壁とIDが同じなら失敗判定にする
			if (CheckContinuanceKick(&hit))
			{
				return false;
			}
			return true;
		}
	}
	for (auto& hit : hitListLeft)
	{
		if (Collsions::CubeCollision(hit, cube))
		{
			//ひとつ前のセーブした壁とIDが同じなら失敗判定にする
			if (CheckContinuanceKick(&hit))
			{
				return false;
			}
			return true;
		}
	}
	return false;
}

void Player::DebugGUI()
{
	ImGui::Text("position x:%f y:%f z:%f",
		position.x, position.y, position.z);

	ImGui::Text("rotation x:%f y:%f z:%f", rotation.x, rotation.y, rotation.z);
	//ImGui::Text("CanWallKick %d", CanWallKick());
	if (saveKickWall != nullptr)
	{
		ImGui::Text("wallID %d", saveKickWall->GetID());
	}
	if (ImGui::Button("FlyChange"))
	{
		flyMode = !flyMode;
		if(flyMode)SetNoGravity(true);
		if(!flyMode)SetNoGravity(false);
	}
	ImGui::Text("flyMode %d", flyMode);
	ImGui::Text("quaternion x:%f y:%f z:%f w:%f", quaternion.vector.x, quaternion.vector.y, quaternion.vector.z, quaternion.w);
	ImGui::Text("endRota x:%f y:%f z:%f", endRota.x, endRota.y, endRota.z);
	ImGui::Text("startQ x:%f y:%f z:%f w:%f", startQ.vector.x, startQ.vector.y, startQ.vector.z, startQ.w);
	ImGui::Text("endQ x:%f y:%f z:%f w:%f", endQ.vector.x, endQ.vector.y, endQ.vector.z, endQ.w);
	ImGui::Text("culQ x:%f y:%f z:%f w:%f", culQ.vector.x, culQ.vector.y, culQ.vector.z, culQ.w);
	//Vector3 testRota;
	//Quaternion test = DirectionToDirection({ 0,0,0 }, {});
	//ImGui::Text("test x:%f y:%f z:%f w:%f", test.vector.x, test.vector.y, test.vector.z, test.w);
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
		SetNoGravity(false);
		SetNoMove(false);
	}
}

void Player::Jump()
{
	//ダッシュ状態ならカウント加算してジャンプ力上げる
	if (playerState == PlayerState::Dash)
	{
		jumpCount++;
	}

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

