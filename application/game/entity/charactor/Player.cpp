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

bool Player::CheckState(PlayerState check)
{
	for (auto& tag : playerStates)
	{
		if (tag == check)
		{
			return true;
		}
	}
	return false;
}

bool Player::SetState(PlayerState check)
{
	for (auto itr = playerStates.begin(); itr != playerStates.end(); itr++)
	{
		if (*itr == check)
		{
			return false;
		}
	}
	playerStates.push_back(check);
	return true;
}

bool Player::DeleteState(PlayerState check)
{
	for (auto itr = playerStates.begin(); itr != playerStates.end(); itr++)
	{
		if (*itr == check)
		{
			playerStates.erase(itr);
			return true;
		}
	}

	return false;
}

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

	mModelOffset = { 0,0.3f,0 };

	SetState(PlayerState::Normal);
}

void Player::Update()
{
	ApparanceUpdate();

	//移動地を初期化
	moveValue = { 0,0,0 };

	//ベクトルを保存
	mCenterVec = centerObject.matWorld.ExtractAxisZ();
	mSideVec = centerObject.matWorld.ExtractAxisX();

	//ダッシュ変更
	ChangeDash();
	//通常の状態であれば
	NormalUpdate();
	//ダッシュ状態であれば
	DashUpdate();

	HipDropUpdate();
	
	//デバッグ状態のいろいろ
	DebugUpdate();
	
	AdjudicationUpdate();

	//回転更新
	RotaUpdate();

	//更新
	Obj3d::Update(*Camera::sCamera);

	//当たり判定後、ステータスの更新
	DamageUpdate();

	hpGauge.Update();
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
			wallKickVec.y = 0;
		}
	}
	//ジャンプ中なら
	if (IsJump())
	{
		//回転する
		if (jumpCount == 2)
		{
			jumpRotaX -= 18;
		}
		else if (jumpCount >= 3)
		{
			jumpRotaX += 36;
		}
		//ダッシュ状態でなくなったらもとの回転に戻す
		if (!CheckState(PlayerState::Dash)) {
			jumpRotaX = 0;
		}
	}

	if (wallKickTimer.GetRun())
	{
		jumpRotaX -= 18;
	}

	//着地してるなら取り消す
	if (!IsJump())
	{
		if (jumpRotaX != 0) {
			jumpRotaX = 0;
			PlayerCamera::Get()->InitRadius();
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

	//⑶終点Qを求める
	culQ = Slerp(startQ,endQ, rotTime.GetTimeRate());

	Quaternion rotX = MakeAxisAngle({1,0,0}, MathF::AngleConvRad(jumpRotaX));
	Quaternion rotY = MakeAxisAngle({0,1,0}, MathF::AngleConvRad(jumpRotaY));

	//④現在のプレイヤーの向き(Q)に足しまくる
	quaternion = culQ * rotX;
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
	return (jumpState == JumpState::Up || jumpState == JumpState::Staying || jumpState == JumpState::Down);
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
	ChangeState(PlayerState::Apparrance);

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

void Player::ChangeDash()
{
	if (Input::Keyboard::PushKey(DIK_LSHIFT)) {
		SetState(PlayerState::Dash);
	}
	else {
		DeleteState(PlayerState::Dash);
	}
	//ヒップドロップ中ならダッシュを解除する
	if (CheckState(PlayerState::HipDrop)) {
		DeleteState(PlayerState::Dash);
	}
}

void Player::NormalUpdate()
{
	if (CheckState(PlayerState::Normal))
	{
		if (CheckState(PlayerState::Dash)) {
			mSpeed = DASH_SPEED;
		}
		else {
			mSpeed = NORMAL_SPEED;
			jumpCount = 0;
		}

		MoveUpdate();
	}
}

void Player::DashUpdate()
{
	Vector3 playerBack{};
	Vector3 playerFeetPos{};

	if (CheckState(PlayerState::Dash))
	{
		//プレイヤーの少し後ろの足元座標を算出
		playerBack = -matWorld.ExtractAxisZ() * (scale.z / 2);
		playerFeetPos = Vector3(position.x, position.y - (scale.y / 2), position.z) + playerBack;

		//パーティクル配置
		ParticleManager::Get()->CreateCubeParticle(
			playerFeetPos,
			{ 0.5f ,0.5f ,0.5f },
			1.0f,
			{ 1.0f,1.0f,1.0f,1.0f }
		);
	}
}

void Player::DebugUpdate()
{
	//デバッグ変更
	if (Input::Keyboard::TriggerKey(DIK_1))
	{
		if (CheckState(PlayerState::Debug)) {
			DeleteState(PlayerState::Debug);
		}
		else {
			SetState(PlayerState::Debug);
		}
	}

	if (CheckState(PlayerState::Debug))
	{
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

		//入力でリロード
		if (Input::Keyboard::TriggerKey(DIK_R))
		{
			StageChanger::Get()->Reload();
		}
	}

}

void Player::ApparanceUpdate()
{
	apparranceTimer.Update();
	if (CheckState(PlayerState::Apparrance))
	{
		Vector3 endpos = saveDokanPos + Vector3(0, saveDokanScale.y / 2.f, 0);
		position = TEasing::OutQuad(saveDokanPos, endpos, apparranceTimer.GetTimeRate());

		if (apparranceTimer.GetEnd())
		{
			ChangeState(PlayerState::Normal);

			//ここら辺システム側の処理だから、別の場所に移したい
			GameUIManager::Get()->Move(UIMove::START, "StageTitle");
		}
	}
}

void Player::HipDropUpdate()
{
	//ヒップドロップの更新
	hipDropTimer.Update();
	if (IsJump())
	{
		//ヒップドロップ処理
		if (Input::Keyboard::TriggerKey(DIK_LSHIFT))
		{
			if (!CheckState(PlayerState::HipDrop)) {
				SetState(PlayerState::HipDrop);
				hipDropTimer.Start();
				PlayerCamera::Get()->ChangeRadius(1.0f, hipDropTimer.mMaxTime);
			}
		}
	}
	//地面に着地していて
	if (!IsJump())
	{
		if (CheckState(PlayerState::Normal))
		{
			ResetGravity();
			SetNoMove(false);
		}
		if (CheckState(PlayerState::HipDrop)) {
			DeleteState(PlayerState::HipDrop);
			//エフェクト出す
			for (int i = 0; i < 10; i++)
			{
				ParticleManager::Get()->CreateCubeParticle(position,
					{ 1,1,1 }, 5, { 1,1,1,1 });
			}
		}
	}
	if (hipDropTimer.GetRun())
	{
		jumpRotaX = TEasing::OutQuad(0, 720, hipDropTimer.GetTimeRate());

		SetNoMove(true);
		SetNoGravity(true);
	}
	if (hipDropTimer.GetNowEnd())
	{
		jumpRotaX = 0;

		SetGravity(4.5f);
		PlayerCamera::Get()->InitRadius();

		SetNoGravity(false);
	}
}

void Player::AdjudicationUpdate()
{	
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

	colDrawer.Update(*Camera::sCamera);

	//進行方向管理オブジェクトの更新
	centerObject.position = position;
	centerObject.scale = scale;
	centerObject.Update(*Camera::sCamera);
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
	ImGui::Text("jumpState %d", jumpState);
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

	if (ImGui::Button("PopStar")) {
		Star::PopStar(position);
	}
}

bool Player::GetInputMove(InputMove input)
{
	switch (input)
	{
	case Player::InputMove::Push:
		return (Input::Keyboard::PushKey(DIK_W) || 
			Input::Keyboard::PushKey(DIK_S) ||
			Input::Keyboard::PushKey(DIK_A) ||
			Input::Keyboard::PushKey(DIK_D));
		break;
	case Player::InputMove::Trigger:
		return (Input::Keyboard::TriggerKey(DIK_W) ||
			Input::Keyboard::TriggerKey(DIK_S) ||
			Input::Keyboard::TriggerKey(DIK_A) ||
			Input::Keyboard::TriggerKey(DIK_D));
		break;
	}

	return false;
}

void Player::ChangeState(const PlayerState& pState)
{
	SetState(pState);

	if (pState == PlayerState::Normal)
	{
		SetNoCollsion(false);
		SetNoGravity(false);
		SetNoMove(false);

		DeleteState(PlayerState::Apparrance);
		DeleteState(PlayerState::InDokan);
	}

	if (pState == PlayerState::Apparrance)
	{
		SetNoCollsion(true);
		SetNoGravity(true);
		SetNoMove(true);
	}	
	if (pState == PlayerState::InDokan)
	{
		SetNoCollsion(true);
		SetNoGravity(true);
		SetNoMove(true);

		DeleteState(PlayerState::Normal);
	}
	if (pState == PlayerState::Debug)
	{
		SetNoCollsion(false);
		SetNoGravity(false);
		SetNoMove(false);
	}
}

void Player::Jump()
{
	//ダッシュ状態ならカウント加算してジャンプ力上げる
	if (CheckState(PlayerState::Dash))
	{
		jumpCount++;
	}

	jumpCount = Util::Clamp(jumpCount, 0, 3);

	if (jumpCount <= 1)
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
		//この処理だとここが何回も呼ばれて無限にカメラがひいちゃう
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
