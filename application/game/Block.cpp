#include "Block.h"
#include "ViewProjection.h"

void Block::Initialize()
{
	model = ModelManager::GetModel("Cube");
	texture = TextureManager::GetTexture("default");

	ExtractVec();
}

void Block::Update()
{
	Obj3d::Update(*Camera::mCamera);
	cubeCol.position = position;
	cubeCol.scale = scale;
}

bool Block::CheckDirections(const Cube& cubeCol, const CheckDirection& CD) const
{
	switch (CD)
	{
	case CheckDirection::CD_UP:
		return cubeCol.position.y > position.y + scale.y * 0.5f;
		break;
	case CheckDirection::CD_DOWN:
		return cubeCol.position.y < position.y - scale.y * 0.5f;
		break;
	case CheckDirection::CD_CENTER:
		return cubeCol.position.z > position.z + scale.z * 0.5f;;
		break;
	case CheckDirection::CD_BACK:
		return cubeCol.position.z < position.z - scale.z * 0.5f;
		break;
	case CheckDirection::CD_LEFT:
		return cubeCol.position.x > position.x + scale.x * 0.5f;
		break;
	case CheckDirection::CD_RIGHT:
		return cubeCol.position.x < position.x - scale.x * 0.5f;
		break;
	}
	return false;
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
