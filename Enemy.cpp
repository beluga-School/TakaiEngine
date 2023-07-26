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

	///---Œ©‚½–Úˆ—
	//’Ç‚¢‚©‚¯‚é•ûŒü‚ÖŒü‚«‚ğ•Ï‚¦‚é
	tVec = target.position - position;
	tVec.normalize();
	tVec.y = 0;

	return tVec;
}

void Enemy::TargetTurn(const Mob& target, const Vector3& tVec)
{
	Vector3 standardRotaVec = { MathF::PIf / 2,0,0 };

	rotation.y = standardRotaVec.Radian(tVec);
	if (tVec.z > 0) {
		//–³—‚â‚èŒü‚«‚ğ”½‘Î‚É
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
	//’ÇÕ”ÍˆÍ‚Ì‹…‚Ì”¼Œa(ƒx[ƒX‚Ì‘å‚«‚³+‘å‚«‚³‚Ì•½‹Ï‚ğ‘«‚·)
	sphereCol.radius = 8 + MathF::Avarage(scale);

	encountSphere.position = sphereCol.center;
	encountSphere.scale = { sphereCol.radius,sphereCol.radius,sphereCol.radius };

	encountSphere.Update(*Camera::sCamera);
}

void Enemy::EncountSphereDraw(const std::string& oldPipeline)
{
	BasicObjectPreDraw(PipelineManager::GetPipeLine("WireFrame"));
	encountSphere.Draw();
	
	BasicObjectPreDraw(PipelineManager::GetPipeLine(oldPipeline));
}
