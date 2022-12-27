#include "Block.h"
#include "ViewProjection.h"

void Block::Initialize()
{
	model = &ModelManager::GetInstance()->cubeM;
	texture = &TextureManager::GetInstance()->def;
}

void Block::Update()
{
	Obj3d::Update(Camera::camera->matView, Camera::camera->matProjection);
	cubeCol.position = position;
	cubeCol.scale = scale;
}
