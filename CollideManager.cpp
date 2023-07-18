#include "CollideManager.h"
#include "TimeManager.h"

bool CollideManager::CheckDirections(const Cube& check, const Cube& collide, const CheckDirection& CD)
{
	switch (CD)
	{
	case CheckDirection::CD_UP:
		return check.position.y > collide.position.y + collide.scale.y * 0.5f;
		break;
	case CheckDirection::CD_DOWN:
		return check.position.y < collide.position.y - collide.scale.y * 0.5f;
		break;
	case CheckDirection::CD_CENTER:
		return check.position.z > collide.position.z + collide.scale.z * 0.5f;;
		break;
	case CheckDirection::CD_BACK:
		return check.position.z < collide.position.z - collide.scale.z * 0.5f;
		break;
	case CheckDirection::CD_LEFT:
		return check.position.x > collide.position.x + collide.scale.x * 0.5f;
		break;
	case CheckDirection::CD_RIGHT:
		return check.position.x < collide.position.x - collide.scale.x * 0.5f;
		break;
	}
	return false;
}

void Box::CreateCol()
{
	cubecol.position = position;
	cubecol.scale = scale;
}

void Box::CreateCol(const Vector3& pos_, const Vector3& scale_)
{
	cubecol.position = pos_;
	cubecol.scale = scale_;
}

void Box::Register()
{
	CollideManager::Get()->allCols.push_back(this);
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
			gravity += gravityAdd;
			position.y -= gravity * TimeManager::deltaTime;
		}
		//hitListオブジェクトの中で、最も高い位置にあるオブジェクトに自身が当たっているなら
		else
		{
			jumpState = JumpState::None;
		}

		break;
	}
}

void CollideManager::Update()
{
	//同じリスト内の他のオブジェクト全ての数分判定を回す
	for (Box*check : allCols)
	{
		for (Box* collide : allCols)
		{
			CheckCollide(check, collide);
		}
	}
}

void CollideManager::CheckCollide(Box* check, Box* collide)
{
	//同じなら判定しないぜ
	if (check == collide)
	{
		return;
	}

	//判定する側がモブの時
	if (check->tag == TagTable::Mob)
	{
		if (check == nullptr)
		{
			return;
		}
		//される側がブロックなら
		if (collide->tag == TagTable::Block)
		{
			//checkがmobであることは確定しているので、mob型に変換してデータを持ってくる
			Mob* mob = static_cast<Mob*>(check);

			//collideがBlockであることは確定しているので、Block型に変換してデータを持ってくる
			Block* block = static_cast<Block*>(collide);

			//押し戻し処理を行う
			Osimodosi(*mob, *block);
		}
	}
}

void CollideManager::Osimodosi(Mob& check, const Block& collide)
{
	//そのオブジェクトより
	//上にいるか
	bool up = CheckDirections(check.cubecol, collide.cubecol, CheckDirection::CD_UP);
	//下にいるか
	bool down = CheckDirections(check.cubecol, collide.cubecol, CheckDirection::CD_DOWN);
	//前にいるか
	bool center = CheckDirections(check.cubecol, collide.cubecol, CheckDirection::CD_CENTER);
	//後ろにいるか
	bool back = CheckDirections(check.cubecol, collide.cubecol, CheckDirection::CD_BACK);
	//左にいるか
	bool left = CheckDirections(check.cubecol, collide.cubecol, CheckDirection::CD_LEFT);
	//右にいるか
	bool right = CheckDirections(check.cubecol, collide.cubecol, CheckDirection::CD_RIGHT);

	//上面の当たり判定
	if (up)
	{
		//直線状で見たときに下にあるオブジェクトがあれば
		Cube rayCube;
		rayCube.position = check.position;
		rayCube.scale = check.scale;
		//スケールをめっちゃ引き延ばす
		rayCube.scale.y = 100;

		//当たったなら
		//生き物=無機物
		bool cubeCol = Collsions::CubeCollision(rayCube, collide.cubecol);

		if (cubeCol)
		{
			//リストに入れる

			//このリストをいちいち消すのではなく、
			//当たり判定をとり、同じ要素が入っていないなら入れて
			//当たり判定が外れたときに、その要素を消す
			UniqueObjectPushBack(check.hitListY, collide.cubecol);
		}
		else
		{
			UniqueObjectErase(check.hitListY, collide.cubecol);
		}
	}
	

	//下面の当たり判定
	if (down)
	{

	}

	//左右の当たり判定
	if (up == false)
	{
		//左右も別の当たり判定リスト作って、同じ手法で取れそう？
		if (Collsions::CubeCollision(check.cubecol, collide.cubecol))
		{
			//横方向を少し大きくして、当たり判定を取ったオブジェクトと当たっているなら
			if (right)
			{
				check.moveValue.x = 0;
			}
			if (left)
			{
				check.moveValue.x = 0;
			}
			if (back)
			{
				check.moveValue.z = 0;
			}
			if (center)
			{
				check.moveValue.z = 0;
			}
		}
	}
}
