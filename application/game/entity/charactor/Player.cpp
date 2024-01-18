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
#include "LightGroup.h"

using namespace Input;

bool Player::CheckState(PlayerState check)
{
	for (auto& tag : mPlayerStates)
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
	for (auto itr = mPlayerStates.begin(); itr != mPlayerStates.end(); itr++)
	{
		if (*itr == check)
		{
			return false;
		}
	}
	mPlayerStates.push_back(check);
	return true;
}

bool Player::DeleteState(PlayerState check)
{
	for (auto itr = mPlayerStates.begin(); itr != mPlayerStates.end(); itr++)
	{
		if (*itr == check)
		{
			mPlayerStates.erase(itr);
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

	mColDrawer.Initialize();
	mColDrawer.SetModel(ModelManager::GetModel("ICOSphere"));
	mColDrawer.SetTexture(TextureManager::GetTexture("white"));

	mCenterObject.Initialize();
	mCenterObject.SetModel(ModelManager::GetModel("beetle"));

	SetOutLineState({ 0.1f,0.1f,0.1f,1.0f }, 0.05f);

	mHpGauge.Initialize();

	rotmode = RotMode::Quaternion;
	mCenterObject.rotmode = RotMode::Quaternion;

	quaternion = DirectionToDirection({0,0,0},{0,0,1});

	mModelOffset = { 0,0.3f,0 };

	mPlayerStates.clear();
	SetState(PlayerState::Normal);

	SetNoMove(false);
	SetNoCollsion(false);
	SetNoGravity(false);

	mEncountCol.center = position;
	mEncountCol.radius = 0;
}

void Player::Update()
{
	ApparanceUpdate();

	//移動地を初期化
	oldMove = moveValue;
	moveValue = { 0,0,0 };

	//ベクトルを保存
	mCenterVec = mCenterObject.matWorld.ExtractAxisZ();
	mSideVec = mCenterObject.matWorld.ExtractAxisX();

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

	mHpGauge.Update();

	oldPad = Pad::GetLStickMove();

}

void Player::CheckGUI()
{
	ImGui::Text("pPosY %f", position.y);
	for (auto& down : hitListDown)
	{
		ImGui::Text("hitblockID %d", down.GetID());
		ImGui::SliderFloat("hitblockX", &down.position.x,-1000,1000);
		ImGui::SliderFloat("hitblockY", &down.position.y,-100,100);
		ImGui::SliderFloat("hitblockZ", &down.position.z,-1000,1000);
	}
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
	mHpGauge.Draw();
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
	mWallKickTimer.Update();
	
	//縦移動更新(重力落下含む)
	//ジャンプしていない状態で入力があったらジャンプ
	if (jumpState == JumpState::None)
	{
		if (Input::Keyboard::TriggerKey(DIK_SPACE) ||
			Pad::TriggerPadButton(PadButton::A))
		{
			Jump();
			GroundHitParticle();
		}
	}
	if (CanWallKick())
	{
		//if (Input::Keyboard::TriggerKey(DIK_SPACE) ||
		//	Pad::TriggerPadButton(PadButton::A))
		//{
		//	Jump();
		//	//反対方向に飛ぶ
		//	mWallKickTimer.Start();
		//	mWallKickVec = matWorld.ExtractAxisZ();
		//	mWallKickVec.y = 0;
		//}
	}

	//ジャンプ中なら
	if (IsJump())
	{
		//回転する
		if (mJumpCount == 2)
		{
			mJumpRotaX -= ROTA_VALUE;
		}
		else if (mJumpCount >= 3)
		{
			mJumpRotaX += ROTA_VALUE * 2;
		}
		//ダッシュ状態でなくなったらもとの回転に戻す
		if (!CheckState(PlayerState::Dash)) {
			mJumpRotaX = 0;
		}
	}

	if (mWallKickTimer.GetRun())
	{
		mJumpRotaX -= ROTA_VALUE;
	}

	//着地してるなら取り消す
	if (!IsJump())
	{
		if (mJumpRotaX != 0) {
			mJumpRotaX = 0;
			PlayerCamera::Get()->InitRadius();
		}
	}

	if (CheckNowHitGround())
	{
		GroundHitParticle();
	}

	if (GetInputMove(Player::InputMove::Push))
	{
		moveValue += mCenterVec * mSpeed * TimeManager::deltaTime;
	}

	//壁キックタイマーが動いてるなら壁キック入力があったときの逆進行方向を足し続ける
	if (mWallKickTimer.GetRun())
	{
		moveValue = { 0,0,0 };
		//斜め上に飛ばしたい
		moveValue -= mWallKickVec;
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
	mRotTime.Update();
	mRotTime.mMaxTime = 0.5f;

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

	if (GetInputMove(Player::InputMove::Trigger))
	{
		mRotTime.Start();
		mEndRota = { 0,0,0 };
	}

	if (Input::Pad::GetLStickMove().GetLength() > 0)
	{
		if (mRotTime.GetEnd())
		{
			mRotTime.Reset();
		}
		mRotTime.NoInitStart();
		if (Input::Pad::GetLStickMove().y > 0)mEndRota += center;
		if (Input::Pad::GetLStickMove().y < 0)mEndRota += back;
		if (Input::Pad::GetLStickMove().x > 0)mEndRota += right;
		if (Input::Pad::GetLStickMove().x < 0)mEndRota += left;
	}
	else
	{
		if (Input::Keyboard::PushKey(DIK_W))mEndRota += center;
		if (Input::Keyboard::PushKey(DIK_S))mEndRota += back;
		if (Input::Keyboard::PushKey(DIK_D))mEndRota += right;
		if (Input::Keyboard::PushKey(DIK_A))mEndRota += left;
	}
	
	mEndQ = DirectionToDirection({ 0,0,1 }, mEndRota);

	//⑵始点になる今のプレイヤーの向きをクオータニオンで取得する
	Vector3 startRota;
	//回転を打ち消した始点のベクトルを作成
	startRota = mCenterObject.matWorld.ExtractAxisZ();
	//startRota = {0,0,1};
	startRota.y = 0;

	mStartQ = DirectionToDirection({ 0,0,1 }, startRota);

	//⑶終点Qを求める
	mCulQ = Slerp(mStartQ, mEndQ, mRotTime.GetTimeRate());

	Quaternion rotX = MakeAxisAngle({ 1,0,0 }, MathF::AngleConvRad(mJumpRotaX));
	Quaternion rotY = MakeAxisAngle({ 0,1,0 }, MathF::AngleConvRad(mJumpRotaY));

	//④現在のプレイヤーの向き(Q)に足しまくる
	quaternion = mCulQ * rotX;
	mCenterObject.quaternion = mCulQ;
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
	mMutekiTimer.Update();
	mFlashTimer.Update();

	//無敵時間中は点滅させる
	if (mMutekiTimer.GetRun())
	{
		if (mFlashTimer.GetRun() == false)
		{
			mFlashTimer.Start();
			mIsVisiable = !mIsVisiable;
		}
	}
	//無敵時間が終了したら点滅解除
	if (mMutekiTimer.GetEnd())
	{
		if (mIsVisiable == false)
		{
			mIsVisiable = true;
		}
		mMutekiTimer.Reset();
	}

	//外側でHPの最大値を超える処理を書いていたら丸める
	mHp.mCurrent = Util::Clamp(mHp.mCurrent, -1, MAX_HP);

	//ダメージを受けたとき
	if (mHp.DecreaseTrigger())
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
	
	if (mHp.DecreaseTrigger())
	{
		changeIndex = (MAX_HP - mHp.mCurrent) - 1;
		changeIndex = Util::Clamp(changeIndex, 0, MAX_HP);
		
		mHpGauge.Addition(-1);
	}
	if (mHp.IncreaseTrigger())
	{
		changeIndex = (MAX_HP - mHp.mCurrent);
		changeIndex = Util::Clamp(changeIndex, 0, MAX_HP);

		mHpGauge.Addition(1);
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
	if (mMutekiTimer.GetRun())return;

	//無敵時間開始
	mMutekiTimer.Start();
	//ダメージ受ける
	mHp.mCurrent -= damage;
}

void Player::ApparranceMove(const Vector3& dokanPos, const Vector3& dokanScale)
{
	ChangeState(PlayerState::Apparrance);

	mApparranceTimer.Start();
	mSaveDokanPos = dokanPos;
	mSaveDokanScale = dokanScale;
}

bool Player::GetApparanceEnd()
{
	return mApparranceTimer.GetEnd();
}

void Player::HPOverFlow(int32_t value)
{
	mHp.mCurrent = value;
	//HPのオーバーフロー処理
	if (mHp.mCurrent > MAX_HP)
	{
		MAX_HP = mHp.mCurrent;
		mHpGauge.SetGaugeSize(mHp.mCurrent, true);
	}
}

bool Player::CheckContinuanceKick(IDdCube* check)
{
	//ひとつ前のセーブした壁とIDが同じならtrueを返す
	if (mSaveKickWall != nullptr)
	{
		if (mSaveKickWall->GetID() == check->GetID())
		{
			return true;
		}
	}
	//IDが不正な物でなければ
	if (mSaveKickWall == nullptr)
	{
 		mSaveKickWall = check;
	}
	return false;
}

void Player::ChangeDash()
{
	if (Input::Keyboard::PushKey(DIK_LSHIFT)) {
		SetState(PlayerState::Dash);
		if (Input::Keyboard::TriggerKey(DIK_LSHIFT))
		{
			DonuteSmoke(position);
		}
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
			mJumpCount = 0;
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

		Vector3 randPos = Util::GetRandVector3(playerFeetPos, -0.75f, 0.75f, { 1,0,1 });

		float time = MathF::GetRand(0.5f, 1.0f);
		float size = MathF::GetRand(0.1f, 0.3f);

		//パーティクル配置
		ParticleManager::Get()->CreateSmoke(
			randPos,
			randPos,
			{ size ,size ,size },
			time,
			Color(1,1,1,1),
			EASEPATTERN::INBACK);
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
		if (mFlyMode)
		{
			Fly();
		}

		//ダメージ処理テスト
		if (Input::Keyboard::TriggerKey(DIK_T))
		{
			mMutekiTimer.Reset();
			DamageEffect(1);
		}
		if (Input::Keyboard::TriggerKey(DIK_G))
		{
			mHp.mCurrent += 1;
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
	mApparranceTimer.Update();
	if (CheckState(PlayerState::Apparrance))
	{
		Vector3 endpos = mSaveDokanPos + Vector3(0, mSaveDokanScale.y / 2.f, 0);
		position = TEasing::OutQuad(mSaveDokanPos, endpos, mApparranceTimer.GetTimeRate());

		if (mApparranceTimer.GetEnd())
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
	mHipDropTimer.Update();
	if (IsJump())
	{
		//ヒップドロップ処理
		if (Input::Keyboard::TriggerKey(DIK_LSHIFT))
		{
			if (!CheckState(PlayerState::HipDrop)) {
				SetState(PlayerState::HipDrop);
				mHipDropTimer.Start();
				PlayerCamera::Get()->ChangeRadius(1.0f, mHipDropTimer.mMaxTime);
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
	if (mHipDropTimer.GetRun())
	{
		mJumpRotaX = TEasing::OutQuad(0, 720, mHipDropTimer.GetTimeRate());

		SetNoMove(true);
		SetNoGravity(true);
	}
	if (mHipDropTimer.GetNowEnd())
	{
		mJumpRotaX = 0;

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

	mColDrawer.position = position;
	mColDrawer.scale = scale;

	mColDrawer.Update(*Camera::sCamera);

	//進行方向管理オブジェクトの更新
	mCenterObject.position = position;
	mCenterObject.scale = scale;
	mCenterObject.Update(*Camera::sCamera);
}

void Player::GroundHitParticle()
{
	for (int32_t i = 0; i < 5; i++)
	{
		float size = MathF::GetRand(0.1f, 0.3f);
		float time = MathF::GetRand(0.3f,0.7f);
		Vector3 partPos = Util::GetRandVector3(position, -1.f, 1.f, { 1,0,1 });
		
		ParticleManager::Get()->CreateSmoke(partPos, partPos,
			{ size,size,size }, time, Color(1, 1, 1, 1), EASEPATTERN::INBACK);
	}
}

void Donute(const Obj3d& obj, float x, float y, float z)
{
	Vector3 partPos = obj.position
		+ obj.matWorld.ExtractAxisX() * x
		+ obj.matWorld.ExtractAxisY() * y
		+ obj.matWorld.ExtractAxisZ() * z;

	partPos = Util::GetRandVector3(partPos, -0.25f, 0.25f);
	Vector3 end = partPos - obj.matWorld.ExtractAxisZ() * 2.0f;

	float size = MathF::GetRand(0.5f, 0.7f);
	float time = MathF::GetRand(0.3f, 0.7f);
	ParticleManager::Get()->CreateSmoke(partPos, end,
		{ size,size,size }, time, Color(1, 1, 1, 1), EASEPATTERN::INBACK);
};

void Player::DonuteSmoke(Vector3 center)
{
	center;
	//めっちゃ無理やりだけど、それぞれの角度ごとに足し算引き算して、最終地点を割り出す
	for (int32_t i = 0; i < 4; i++)
	{
		Donute(*this, 0, 1, 1);
		Donute(*this, 0, -1, 1);
		Donute(*this, 1, 0, 1);
		Donute(*this, -1, 0, 1);
		Donute(*this, 0.75f, 0.75f, 1);
		Donute(*this, 0.75f, -0.75f, 1);
		Donute(*this, -0.75f, 0.75f, 1);
		Donute(*this, -0.75f, -0.75f, 1);
	}
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
	
	if (ImGui::Button("FlyChange"))
	{
		mFlyMode = !mFlyMode;
		if(mFlyMode)SetNoGravity(true);
		if(!mFlyMode)SetNoGravity(false);
	}
	ImGui::Text("flyMode %d", mFlyMode);
	
	ImGui::Text("noGravity:%d", noGravity);
	ImGui::Text("mNoMove:%d", mNoMove);
	ImGui::Text("mNoCollision:%d", mNoCollision);

	ImGui::Text("Pad::GetLStickMove.x:%f", Pad::GetLStickMove().x);
	ImGui::Text("Pad::GetLStickMove.y:%f", Pad::GetLStickMove().y);
	ImGui::Text("mEndRota.x:%f", mEndRota.x);
	ImGui::Text("mEndRota.y:%f", mEndRota.y);
	ImGui::Text("mEndRota.z:%f", mEndRota.z);
}

bool Player::GetInputMove(InputMove input)
{
	switch (input)
	{
	case Player::InputMove::Push:
		return (Input::Keyboard::PushKey(DIK_W) || 
			Input::Keyboard::PushKey(DIK_S) ||
			Input::Keyboard::PushKey(DIK_A) ||
			Input::Keyboard::PushKey(DIK_D)) || 
			Input::Pad::GetLStickMove().GetLength() > 0;
		break;
	case Player::InputMove::Trigger:
		return (Input::Keyboard::TriggerKey(DIK_W) ||
			Input::Keyboard::TriggerKey(DIK_S) ||
			Input::Keyboard::TriggerKey(DIK_A) ||
			Input::Keyboard::TriggerKey(DIK_D)) ||
			(Input::Pad::GetLStickMove().GetLength() > 0 && oldMove.length() == 0);
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
		mJumpCount++;
	}

	mJumpCount = Util::Clamp(mJumpCount, 0, 3);

	if (mJumpCount <= 1)
	{
		jumpPower = JUMP_VALUE * 2;
	}

	if (mJumpCount == 2)
	{
		jumpPower = JUMP_VALUE * 3;
		//カメラをちょい引く(もっと放してもいいかも)
		PlayerCamera::Get()->ChangeRadius(1.0f, 0.5f);
	}

	if (mJumpCount == 3)
	{
		jumpPower = JUMP_VALUE * 4;
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
	return mHp.mCurrent;
}
