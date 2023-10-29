#include "Enemy.h"
#include "MathF.h"
#include "ClearDrawScreen.h"

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
}

void Enemy::EncountSphereUpdate()
{
	sphereCol.center = position;
	//追跡範囲の球の半径(ベースの大きさ+大きさの平均を足す)
	sphereCol.radius = 8 + MathF::Avarage(scale);

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
