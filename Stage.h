#pragma once
#include "LevelLoader.h"
#include "Obj.h"
#include "Collision.h"
#include "EventBlock.h"

class Stage
{
public:
	void ChangeLevel(LevelData& data);

	void Update();
	void Draw();

	static Stage* Get()
	{
		static Stage instance;
		return &instance;
	}

	//���f���̔z��
	std::list<Obj3d> mObj3ds;

	//�����蔻��z��
	std::list<Cube> mColCubes;

	//�C�x���g�I�u�W�F�N�g�z��
	std::list<EventBlock> mEventObjects;

private:
	Stage(){};
	~Stage(){};

	//�ʏ�̃I�u�W�F�N�g�z�u
	void NormalObjectSet(const LevelData::ObjectData& data);

	//�����蔻��z�u
	void CollisionSet(const LevelData::ObjectData& data);

	//�C�x���g�I�u�W�F�N�g(�u���b�N�u������)
	void EvenyObjectSet(const LevelData::ObjectData& data);
};

