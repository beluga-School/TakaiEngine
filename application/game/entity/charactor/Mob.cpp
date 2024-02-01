#include "Mob.h"
#include "TimeManager.h"
#include "LightGroup.h"
#include "InstantDrawer.h"

void Mob::CollsionUpdate()
{
	//当たり判定の丸め
	CalcNearestHitLists();

	UpdateY();

	if (moveBlockHit)
	{
		position += moveBlockPosition;
	}
	
	//本加算
	//動けないフラグ立ったら加算しない
	if (mNoMove == false)
	{
		position += moveValue;
	}

	//今後Xも入る予定
	//UpdateX();

	sphereCol.center = position;

	moveBlockPosition = { 0,0,0 };

	moveBlockHit = false;

	//影を動かす
	LightGroup* hoge = LightGroup::Get();
	if (isDead) {
		hoge->mShadow[shadowNum].mActive = false;
	}

	hoge->mShadow[shadowNum].casterPos = position;

	InstantDrawer::DrawBox3D(debugCube);

}

bool Mob::CheckNowHitGround()
{
	return checkGround;
}

void Mob::SetShadow() {
	shadowNum = LightGroup::Get()->CircleShadowActive();
};

void Mob::SetInitScale(const Vector3& scale_)
{
	initScale = scale_;
}

void Mob::SetAddGravity(float value)
{
	gravityAdd = value;
}

void Mob::SetNowGravity(float value)
{
	gravity = value;
}

void Mob::ResetGravity()
{
	gravityAdd = 1.5f;
}

void Mob::UpdateY()
{
	JumpUpdate();
}

void Mob::UpdateX()
{
	//右
	if (position.x <= hitLeftMin)
	{
		position.x = hitLeftMin;
	}
	//左
	if (position.x >= hitRightMin)
	{
		position.x = hitRightMin;
	}
	//正面
	if (position.z >= hitCenterMin)
	{
		position.z = hitCenterMin;
	}
	//正面
	if (position.z <= hitBackMin)
	{
		position.z = hitBackMin;
	}
}

void Mob::CalcNearestHitLists()
{
	//コリジョンを行わないフラグが立っているならスキップ
	if (mNoCollision)return;

	//下方向の判定
	float preDownY = -114514.f;
	float maxDownY = 0;

	hitFeetMax = preDownY;

	for (auto& hit : hitListDown)
	{
		maxDownY = hit.position.y;
		//初期値でなく、前の値より高い位置にあるなら
		if (maxDownY >= preDownY)
		{
			//少しだけ浮かせて、ブロックの切れ目に引っかからないように
			feet = hit.position.y + hit.scale.y / 2;//ここを交点の位置をもらう形にする
			//Q,ここのスケール/2いらなくね
			//A,ここのスケール/2は、判定壁の太さみたいなものなので、固定の太さを持たせればいいと思います
			hitFeetMax = feet + scale.y / 2 + hitOffset;
		}
		preDownY = hit.position.y;
	}

	//上方向の判定
	float preUpY = 114514.f;
	float maxUpY = 0;

	hitCeilingMax = preUpY;

	for (auto& hit : hitListUp)
	{
		maxUpY = hit.position.y;
		if (maxUpY <= preUpY)
		{
			hitCeilingMax = hit.position.y - hit.scale.y / 2 - scale.y / 2;
		}
		preUpY = hit.position.y;
	}

	//X軸判定
	
	//方向の判定
	float preLeft = -114514.f;
	float maxLeft = 0;

	hitLeftMin = preLeft;

	for (auto& hit : hitListLeft)
	{
		maxLeft = hit.position.x;
		if (maxLeft >= preLeft)
		{
			hitLeftMin = hit.position.x + hit.scale.x / 2 + scale.x / 2;
		}
		preLeft = hit.position.x;
	}

	//方向の判定
	float preRight = 114514.f;
	float maxRight = 0;

	hitRightMin = preRight;

	for (auto& hit : hitListRight)
	{
		maxRight = hit.position.x;
		if (maxRight <= preRight)
		{
			hitRightMin = hit.position.x - hit.scale.x / 2 - scale.x / 2;
		}
		preRight = hit.position.x;
	}

	//Z軸判定
	float preCenter = 114514.f;
	float maxCenter = 0;

	hitCenterMin = preCenter;

	for (auto& hit : hitListCenter)
	{
		maxCenter = hit.position.z;
		if (maxCenter <= preCenter)
		{
			hitCenterMin = hit.position.z - hit.scale.z / 2 - scale.z / 2;
		}
		preCenter = hit.position.z;
	}

	float preBack = -114514.f;
	float maxBack = 0;

	hitBackMin = preBack;

	for (auto& hit : hitListBack)
	{
		maxBack = hit.position.z;
		if (maxBack >= preBack)
		{
			hitBackMin = hit.position.z + hit.scale.z / 2 + scale.z / 2;
		}
		preBack = hit.position.z;
	}
}

void Mob::JumpUpdate()
{
	checkGround = false;

	if (mNoCollision)return;

	jumpManageTimer.Update();
	stayManageTimer.Update();

	float mostInter = -999.0f;
	float pFeet = position.y + sphereCol.radius / 2;

	switch (jumpState)
	{
	case Mob::JumpState::None:

		for (auto& info : hitInfos)
		{
			if (mostInter < info.inter.y) {
				mostInter = info.inter.y;
			}
		}

		if (hitInfos.size() >= 1)
		{
			position.y = pFeet;
		}

		//ここの処理が設置状態でも入っちゃう
		if (mostInter < pFeet)
		{
			if (jumpState == Mob::JumpState::None)
			{
				jumpState = Mob::JumpState::Down;
			}
		}

		hitInfos.clear();

		SetNowGravity(0);

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
			jumpState = JumpState::Down;
			stayManageTimer.Reset();
		}

		break;
	case Mob::JumpState::Down:
		if (!noGravity)
		{
			gravity += gravityAdd;
			gravity = Util::Clamp(gravity, 0.f, 30.f);
			moveValue.y -= gravity * TimeManager::deltaTime;
		};
		break;
	}

	if (mostInter > -100.0f)
	{
		debugCube.position = position;
		debugCube.position.y = mostInter;
	}
}