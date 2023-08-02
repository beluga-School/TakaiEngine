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
				//プレイヤーが土管の上にいるなら
				if (CheckDirections(player->box.cubecol, dokan->box.cubecol, CheckDirection::CD_UP))
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
	bool center = CheckDirections(check.box.cubecol, collide.box.cubecol, CheckDirection::CD_CENTER);
	//後ろにいるか
	bool back = CheckDirections(check.box.cubecol, collide.box.cubecol, CheckDirection::CD_BACK);
	//左にいるか
	bool left = CheckDirections(check.box.cubecol, collide.box.cubecol, CheckDirection::CD_LEFT);
	//右にいるか
	bool right = CheckDirections(check.box.cubecol, collide.box.cubecol, CheckDirection::CD_RIGHT);

	//上面の当たり判定
	//直線状で見たときに下にあるオブジェクトがあれば
	Cube rayCube;
	rayCube.position = check.position;
	rayCube.scale = check.scale;
	//スケールをめっちゃ引き延ばす
	rayCube.scale.y = 100;

	//当たったなら
	//生き物=無機物
	bool cubeCol = Collsions::CubeCollision(rayCube, collide.box.cubecol);

	bool upCol = up && cubeCol;

	//全て0なら埋まっている
	bool isBuried = !up && !down && !right && !left && !back && !center;

	//上面が当たっているかオブジェクトが埋まっているなら
	if (upCol || isBuried)
	{
		//リストに入れる
		UniqueObjectPushBack(check.hitListY, collide.box.cubecol);
	}
	else
	{
		//そうでないなら外す
		UniqueObjectErase(check.hitListY, collide.box.cubecol);
	}
	

	//下面の当たり判定
	if (down)
	{

	}

	//左右の当たり判定
	if (up == false)
	{
		//左右も別の当たり判定リスト作って、同じ手法で取れそう？
		if (Collsions::CubeCollision(check.box.cubecol, collide.box.cubecol))
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