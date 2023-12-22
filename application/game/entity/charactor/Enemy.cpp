#include "Enemy.h"
#include "MathF.h"
#include "ClearDrawScreen.h"

bool Enemy::CheckState(ActTable check)
{
	for (auto& tag : actTables)
	{
		if (tag == check)
		{
			return true;
		}
	}
	return false;
}

bool Enemy::SetState(ActTable check)
{
	for (auto itr = actTables.begin(); itr != actTables.end(); itr++)
	{
		if (*itr == check)
		{
			return false;
		}
	}
	actTables.push_back(check);
	return true;
}

bool Enemy::DeleteState(ActTable check)
{
	for (auto itr = actTables.begin(); itr != actTables.end(); itr++)
	{
		if (*itr == check)
		{
			actTables.erase(itr);
			return true;
		}
	}

	return false;
}

void Enemy::ForceState(ActTable force)
{
	actTables.clear();
	SetState(force);
}

int32_t Enemy::GetHitDamage()
{
	return hitDamage;
}

Vector3 Enemy::TargetVector(const Mob& target)
{
	Vector3 tVec{};

	///---見た目処理
	//追いかける方向へ向きを変える
	tVec = target.position - position;
	tVec.normalize();
	tVec.y = 0;

	return tVec;
}

void Enemy::TargetTurn(const Vector3& tVec)
{
	Vector3 standardRotaVec = { MathF::PIf / 2,0,0 };

	rotation.y = standardRotaVec.Radian(tVec);
	if (tVec.z > 0) {
		//無理やり向きを反対に
		rotation.y *= -1;
	}
}

void Enemy::EncountSphereInitialize()
{
	encountSphere.Initialize();
	encountSphere.SetModel(ModelManager::GetModel("ICOSphere"));
	encountSphere.SetTexture(TextureManager::GetTexture("white"));

	sphereCol.center = position;
	sphereCol.radius = 0;
}

void Enemy::EncountSphereUpdate()
{
	sphereCol.center = position;

	encountSphere.position = sphereCol.center;
	encountSphere.scale = { sphereCol.radius,sphereCol.radius,sphereCol.radius };

	encountSphere.Update(*Camera::sCamera);
}

void Enemy::EncountSphereDraw(const std::string& oldPipeline)
{
	BasicObjectPreDraw("WireFrame");
	encountSphere.Draw();
	
	BasicObjectPreDraw(oldPipeline);
}

void Enemy::SetMaxHP(int32_t SET_MAX_HP)
{
	int32_t nowHP = hp.mCurrent;
	int32_t nowMAXHP = maxHP;
	int32_t nowDecreaceHP = nowMAXHP - nowHP;
	maxHP = SET_MAX_HP;
	hp = SET_MAX_HP - nowDecreaceHP;
}

bool Enemy::GetActive()
{
	return mActive;
}
