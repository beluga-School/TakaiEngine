#include "PlayerState.h"
#include "Player.h"
#include "Input.h"
#include "PlayerCamera.h"
#include "TimeManager.h"
#include "ObjParticle.h"
#include "MathF.h"

using namespace Input;

void PlayerNormal::Update(Player* player)
{
	//縦移動更新(重力落下含む)
	//ジャンプしていない状態で入力があったらジャンプ
	if (player->GetJumpState() == Player::JumpState::None)
	{
		if (Keyboard::TriggerKey(DIK_SPACE) ||
			Pad::TriggerPadButton(PadButton::A))
		{
			player->Jump();
			player->GroundHitParticle();
		}
	}

	if (player->CheckNowHitGround())
	{
		player->GroundHitParticle();
	}

	if (player->GetInputMove(Player::InputMove::Push))
	{
		Vector3 plusValue = player->GetCenterVec() * player->GetSpeed() * TimeManager::deltaTime;
		player->MoveValuePlus(plusValue);
	}
}

std::string PlayerNormal::showState()
{
	return "normal";
}

void PlayerDash::Update(Player* player)
{
	Vector3 playerBack{};
	Vector3 playerFeetPos{};

	//プレイヤーの少し後ろの足元座標を算出
	playerBack = -player->matWorld.ExtractAxisZ() * (player->scale.z / 2);
	playerFeetPos = Vector3(player->position.x,
		player->position.y - (player->scale.y / 2),
		player->position.z) + playerBack;

	Vector3 randPos = Util::GetRandVector3(playerFeetPos, -0.25f, 0.25f, { 1,0,1 });

	float time = MathF::GetRand(0.5f, 1.0f);
	float size = MathF::GetRand(0.3f, 0.6f);

	//パーティクル配置
	ParticleManager::Get()->CreateSmoke(
		randPos,
		randPos,
		{ size ,size ,size },
		time,
		Color(1, 1, 1, 1),
		EASEPATTERN::INBACK);

	if (player->GetInputMove(Player::InputMove::Push))
	{
		Vector3 plusValue = player->GetCenterVec() * player->GetSpeed() * TimeManager::deltaTime;
		player->MoveValuePlus(plusValue);
	}
}

std::string PlayerDash::showState()
{
	return "dash";
}
