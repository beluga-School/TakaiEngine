#include "CollideManager.h"
#include "TimeManager.h"
#include "Mob.h"
#include "Block.h"
#include "Cannon.h"
#include "ImguiManager.h"
#include "Player.h"
#include "Enemy.h"
#include "Star.h"
#include "WarpBlock.h"
#include "Dokan.h"
#include "MoveBlock.h"

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

//中身見れなかったので、無理やり死亡を確認できるように関数作成
bool CheckIsDead(Entity* box)
{
	return box->IsDead();
}

//TODO:線形探索でやってる上に死亡確認2回呼んでさらに2重for分2回呼んでるからもうまじであほあほあほあほ
//うるせ～～～～～しらね～～～～～～～～～～
void CollideManager::CollideUpdate()
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

	for (Entity*check : allCols)
	{
		for (Entity* collide : allCols)
		{
			CheckCollide(check, collide);
		}
	}
}

void CollideManager::StatusUpdate()
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

	for (Entity* check : allCols)
	{
		for (Entity* collide : allCols)
		{
			CheckStatus(check, collide);
		}
	}
}

void CollideManager::CheckCollide(Entity* check, Entity* collide)
{
	//同じなら判定しないぜ
	if (check == collide)
	{
		return;
	}

	//判定する側がプレイヤーの時
	if (check->CheckTag(TagTable::Player))
	{
		//checkがPlayerであることは確定しているので、player型に変換してデータを持ってくる
		Player* player = static_cast<Player*>(check);

		if (collide->CheckTag(TagTable::Dokan))
		{
			//collideがDokanであることは確定しているので、Dokan型に変換してデータを持ってくる
			Dokan* dokan = static_cast<Dokan*>(collide);
			
			Cube tempDokan = dokan->box.cubecol;
			tempDokan.scale.y *= 1.1f;

			if (Collsions::CubeCollision(player->box.cubecol, tempDokan))
			{
				if (player->mSetFrame)
				{
					player->ApparranceMove(dokan->box.cubecol.position, dokan->box.cubecol.scale);
					player->mSetFrame = false;
				}
				//プレイヤーが土管の上にいるなら
				else if (CheckDirections(player->box.cubecol, dokan->box.cubecol, CheckDirection::CD_UP))
				{
					//内部の処理を行えるように
					dokan->HitEffect(player);
				}
			}
		}
	}

	//判定する側がモブの時
	if (check->CheckTag(TagTable::Mob))
	{
		if (check == nullptr)
		{
			return;
		}

		//checkがmobであることは確定しているので、mob型に変換してデータを持ってくる
		Mob* mob = static_cast<Mob*>(check);

		//される側がブロックなら
		if (collide->CheckTag(TagTable::Block))
		{
			//collideがBlockであることは確定しているので、Block型に変換してデータを持ってくる
			Block* block = static_cast<Block*>(collide);

			//押し戻し処理を行う
			Osimodosi(*mob, *block);
		}
		if (collide->CheckTag(TagTable::Cannon))
		{
			//collideがCannonであることは確定しているので、Cannon型に変換してデータを持ってくる
			Cannon* cannon = static_cast<Cannon*>(collide);
			if (Collsions::CubeCollision(mob->box.cubecol, cannon->box.cubecol))
			{
				cannon->OnCollide(*mob);
			}
		}
	}
}

void CollideManager::CheckStatus(Entity* check, Entity* collide)
{
	//同じなら判定しないぜ
	if (check == collide)
	{
		return;
	}

	//判定する側がPlayerの時
	if (check->CheckTag(TagTable::Player))
	{
		//checkがPlayerであることは確定しているので、mob型に変換してデータを持ってくる
		Player* player = static_cast<Player*>(check);

		//される側がEnemyなら
		if (collide->CheckTag(TagTable::Enemy))
		{
			//collideがEnemyであることは確定しているので、mob型に変換してデータを持ってくる
			Enemy* enemy = static_cast<Enemy*>(collide);

			CheckPlayerToEnemy(*player, *enemy);
		}
		if (collide->CheckTag(TagTable::Star))
		{
			//collideがStarであることは確定しているので、Star型に変換してデータを持ってくる
			Star* star = static_cast<Star*>(collide);
			if (Collsions::CubeCollision(player->box.cubecol, collide->box.cubecol))
			{
				star->HitEffect();
			}
		}
		if (collide->CheckTag(TagTable::WarpBlock))
		{
			//collideがWarpであることは確定しているので、Warp型に変換してデータを持ってくる
			WarpBlock* warpBlock = static_cast<WarpBlock*>(collide);
			if (Collsions::CubeCollision(player->box.cubecol, warpBlock->box.cubecol))
			{
				warpBlock->HitEffect();
			}
		}
	}
}

void CollideManager::Osimodosi(Mob& check, const Block& collide)
{
	//そのオブジェクトより
	//上にいるか
	bool up = CheckDirections(check.box.cubecol, collide.box.cubecol, CheckDirection::CD_UP);
	//下にいるか
	bool down = CheckDirections(check.box.cubecol, collide.box.cubecol, CheckDirection::CD_DOWN);
	//前にいるか
	bool back = CheckDirections(check.box.cubecol, collide.box.cubecol, CheckDirection::CD_CENTER);
	//後ろにいるか
	bool center = CheckDirections(check.box.cubecol, collide.box.cubecol, CheckDirection::CD_BACK);
	//左にいるか
	bool left = CheckDirections(check.box.cubecol, collide.box.cubecol, CheckDirection::CD_LEFT);
	//右にいるか
	bool right = CheckDirections(check.box.cubecol, collide.box.cubecol, CheckDirection::CD_RIGHT);

	//全て0なら埋まっている
	bool isBuried = !up && !down && !right && !left && !back && !center;

	//Y軸上判定
	Cube rayCubeY;
	rayCubeY.position = check.position;
	rayCubeY.scale = check.scale;
	//スケールをめっちゃ引き延ばす
	rayCubeY.scale.y = 100;

	//当たったなら
	bool cubeColY = Collsions::CubeCollision(rayCubeY, collide.box.cubecol);

	//上面が当たっているかオブジェクトが埋まっているなら
	if ((up && cubeColY) || isBuried)
	{
		//リストに入れる
		UniqueObjectPushBack(check.hitListDown, collide.box.cubecol);
	}
	else
	{
		//そうでないなら外す
		UniqueObjectErase(check.hitListDown, collide.box.cubecol);
	}
	
	//下面の当たり判定
	if (down && cubeColY)
	{
		UniqueObjectPushBack(check.hitListUp, collide.box.cubecol);
	}
	else
	{
		//そうでないなら外す
		UniqueObjectErase(check.hitListUp, collide.box.cubecol);
	}

	//X軸上の判定
	Cube rayCubeX;
	rayCubeX.position = check.position;
	rayCubeX.scale = check.scale;
	//スケールをめっちゃ引き延ばす
	rayCubeX.scale.x = 100;

	//当たったなら
	bool cubeColX = Collsions::CubeCollision(rayCubeX, collide.box.cubecol);

	if (right && cubeColX)
	{
		UniqueObjectPushBack(check.hitListRight, collide.box.cubecol);
	}
	else
	{
		//そうでないなら外す
		UniqueObjectErase(check.hitListRight, collide.box.cubecol);
	}

	if (left && cubeColX)
	{
		UniqueObjectPushBack(check.hitListLeft, collide.box.cubecol);
	}
	else
	{
		//そうでないなら外す
		UniqueObjectErase(check.hitListLeft, collide.box.cubecol);
	}

	//Z軸上の判定
	Cube rayCubeZ;
	rayCubeZ.position = check.position;
	rayCubeZ.scale = check.scale;
	//スケールをめっちゃ引き延ばす
	rayCubeZ.scale.z = 100;

	//当たったなら
	bool cubeColZ = Collsions::CubeCollision(rayCubeZ, collide.box.cubecol);

	if (center && cubeColZ)
	{
		UniqueObjectPushBack(check.hitListCenter, collide.box.cubecol);
	}
	else
	{
		//そうでないなら外す
		UniqueObjectErase(check.hitListCenter, collide.box.cubecol);
	}

	if (back && cubeColZ)
	{
		UniqueObjectPushBack(check.hitListBack, collide.box.cubecol);
	}
	else
	{
		//そうでないなら外す
		UniqueObjectErase(check.hitListBack, collide.box.cubecol);
	}
}

void CollideManager::CheckPlayerToEnemy(Player& player,Enemy& collide)
{
	//踏みつけ判定&接触判定
	if (Collsions::CubeCollision(player.box.cubecol, collide.box.cubecol))
	{
		//死亡モーションに入ってるなら飛ばす
		//今後HPが多い敵が出てきたとき困るから、攻撃を受けた後を表すステートを作るべき
		if (collide.GetNowAct(ActTable::Dead))return;

		if (player.GetJumpState() == Mob::JumpState::Down)
		{
			collide.HitEffect();
			player.Jump();
		}
		else
		{
			//接触ダメージを受ける
			player.DamageEffect(collide.GetHitDamage());
		}
	}

	//エンカウント判定
	if (Collsions::SphereCollsion(player.mEncountCol, collide.sphereCol))
	{
		collide.Encount();
	}
}

void UniqueObjectPushBack(std::list<IDdCube>& list, const IDdCube& col)
{
	for (auto itr = list.begin(); itr != list.end(); itr++)
	{
		//同じ要素が見つかったら止める
		if (itr->GetID() == col.GetID())
		{
			//から入れる
			list.erase(itr);
			break;
		}
		//if (*itr == col)
		//{
		//	return;
		//}
		////回しきれたら同じ要素がない
	}
	
	//から入れる
	list.push_back(col);
}

void UniqueObjectErase(std::list<IDdCube>& list, const IDdCube& col)
{
	for (auto itr = list.begin(); itr != list.end(); itr++)
	{
		//同じ要素が見つかったら
		if (itr->GetID() == col.GetID())
		{
			//消す
			list.erase(itr);
			return;
		}
	}
}
