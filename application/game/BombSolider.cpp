#include "BombSolider.h"
#include "MathF.h"
#include "ObjParticle.h"
#include "Color.h"

void BombSolider::Initialize()
{
	Obj3d::Initialize();
	SetModel(ModelManager::GetModel("bombSolider"));
	SetTexture(TextureManager::GetTexture("white"));
	SetOutLineState({ 0,0,0,1 }, 0.05f);

	EncountSphereInitialize();

	//追跡範囲の球の半径(ベースの大きさ+大きさの平均を足す)
	sphereCol.radius = 8 + MathF::Avarage(scale);

	Register();

	actTables.clear();
	SetState(ActTable::None);
}

void BombSolider::Update()
{
	if (CheckState(ActTable::None)) {
		
	}
	if (CheckState(ActTable::Encount)) {
		for (int32_t i = 0; i < 10; i++)
		{
			ParticleManager::Get()->CreateCubeParticle(position, { 1,1,1 },
				30, { 0, 0, 0, 1 });
		}

		SetState(ActTable::Dead);
		DeleteState(ActTable::Encount);
	}
	if (CheckState(ActTable::Dead)) {
		
	}

	CollsionUpdate();

	EncountSphereUpdate();

	Obj3d::Update(*Camera::sCamera);
}

void BombSolider::Draw()
{
	Obj3d::DrawMaterial();
}

void BombSolider::HitEffect()
{
	if (IsDead())return;
	SetState(ActTable::Dead);

}

void BombSolider::Encount()
{
	//ステートがNoneならエンカウントに以降
	if (!CheckState(ActTable::None)) return;

	SetState(ActTable::Encount);
	DeleteState(ActTable::None);
}
