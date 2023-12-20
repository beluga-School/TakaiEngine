#include "BombSolider.h"
#include "MathF.h"

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

}

void BombSolider::Encount()
{

}
