#include "CollideManager.h"
#include "TimeManager.h"
#include "Mob.h"
#include "Block.h"
#include "Cannon.h"

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

bool CheckIsDead(Box* box)
{
	return box->IsDead();
}

void CollideManager::Update()
{
	//死んでいるなら削除する
	for (auto itr = allCols.begin(); itr != allCols.end();)
	{
		if (CheckIsDead(*itr))
		{
			itr = allCols.erase(itr);
		}
		else
		{
			itr++;
		}
	}
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

		//checkがmobであることは確定しているので、mob型に変換してデータを持ってくる
		Mob* mob = static_cast<Mob*>(check);

		//される側がブロックなら
		if (collide->tag == TagTable::Block)
		{
			//collideがBlockであることは確定しているので、Block型に変換してデータを持ってくる
			Block* block = static_cast<Block*>(collide);

			//押し戻し処理を行う
			//Error:敵の地面押し戻し処理が、死んだ後に行おうとして死んでる感じ
			Osimodosi(*mob, *block);
		}
		if (collide->tag == TagTable::Cannon)
		{
			//collideがCannonであることは確定しているので、Cannon型に変換してデータを持ってくる
			Cannon* cannon = static_cast<Cannon*>(collide);
			if (Collsions::CubeCollision(check->cubecol, collide->cubecol))
			{
				cannon->OnCollide(*mob);
			}
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
