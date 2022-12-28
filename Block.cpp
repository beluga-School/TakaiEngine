#include "Block.h"
#include "ViewProjection.h"

void Block::Initialize()
{
	model = &ModelManager::GetInstance()->cubeM;
	texture = &TextureManager::GetInstance()->def;

	ExtractVec();
}

void Block::Update()
{
	Obj3d::Update(Camera::camera->matView, Camera::camera->matProjection);
	cubeCol.position = position;
	cubeCol.scale = scale;
}

void Block::ExtractVec()
{
	centerVec = matWorld.ExtractAxisZ();
	upVec = matWorld.ExtractAxisY();
	leftVec = matWorld.ExtractAxisX();

	centerVec.normalize();
	upVec.normalize();
	leftVec.normalize();
}
