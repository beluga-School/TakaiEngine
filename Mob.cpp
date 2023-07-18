#include "Mob.h"
#include "TimeManager.h"

void Mob::UpdateY()
{
	JumpUpdate();

	GroundCol();
}

void Mob::GroundCol()
{
	//上方向の判定
	float preY = -114514.f;
	float maxY = 0;

	hitCubeMaxY = preY;

	for (auto& hit : hitListY)
	{
		maxY = hit.position.y;
		//初期値でなく、前の値より高い位置にあるなら
		if (maxY >= preY)
		{
			//一番高い座標を算出
			//少しだけ浮かせて、ブロックの切れ目に引っかからないように
			feet = hit.position.y + hit.scale.y / 2;
			hitCubeMaxY = feet + scale.y / 2 + 0.01f;
		}
		preY = hit.position.y;
	}

	//高さを算出した最大値に合わせる
	if (jumpState == JumpState::None)
	{
		if (position.y > hitCubeMaxY)
		{
			jumpState = JumpState::Down;
		}
		else
		{
			position.y = hitCubeMaxY;
			gravity = 0;
		}
	}
}

void Mob::JumpUpdate()
{
	jumpManageTimer.Update();
	stayManageTimer.Update();

	switch (jumpState)
	{
	case Mob::JumpState::None:

		if (position.y > hitCubeMaxY)
		{
			jumpState = JumpState::Down;
		}
		else
		{
			//地面に立っている状態にする
			gravity = 0;
		}

		break;
	case Mob::JumpState::Up:
		//イージングで上昇
		position.y = TEasing::OutQuad(upJumpS, upJumpE, jumpManageTimer.GetTimeRate());

		//時間が終わったらステートを次の状態に遷移
		if (jumpManageTimer.GetEnd())
		{
			jumpState = JumpState::Staying;
			jumpManageTimer.Reset();
			stayManageTimer.Start();
		}

		break;
	case Mob::JumpState::Staying:
		if (stayManageTimer.GetEnd())
		{
			jumpState = JumpState::None;
			stayManageTimer.Reset();
		}

		break;
	case Mob::JumpState::Down:
		//hitListの中で、最も高い位置にあるオブジェクトより自身の座標が高かったら
		if (position.y > hitCubeMaxY)
		{
			//重力落下させる
			if (!noGravity)
			{
				gravity += gravityAdd;
				position.y -= gravity * TimeManager::deltaTime;
			}
		}
		//hitListオブジェクトの中で、最も高い位置にあるオブジェクトに自身が当たっているなら
		else
		{
			jumpState = JumpState::None;
		}

		break;
	}
}