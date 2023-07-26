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

	///---�����ڏ���
	//�ǂ�����������֌�����ς���
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
		//�����������𔽑΂�
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
	//�ǐՔ͈͂̋��̔��a(�x�[�X�̑傫��+�傫���̕��ς𑫂�)
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
