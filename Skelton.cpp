#include "Skelton.h"

void Skelton::Initialize()
{
	Obj3d::Initialize();
	SetTexture(TextureManager::GetTexture("white"));
	SetOutLineState({ 0,0,0,1 }, 0.05f);

	EncountSphereInitialize();

	Register();

	initScale = scale;
}

void Skelton::Update()
{	
	switch (mActTable)
	{
	case ActTable::None:
		break;
	case ActTable::Encount:
		break;
	case ActTable::Tracking:
		break;
	case ActTable::Dead:
		break;
	}

	//‚¸‚ç‚µ‚½•ª‚ð‰ÁŽZ‚·‚é
	box.CreateCol(position + saveColCenter, box.scale);
	box.ColDrawerUpdate(position + saveColCenter, box.scale);

	CollsionUpdate();

	EncountSphereUpdate();

	Obj3d::Update(*Camera::sCamera);
}

void Skelton::Draw()
{
	Obj3d::DrawMaterial();
}

void Skelton::HitEffect()
{
}

void Skelton::Encount()
{
}
