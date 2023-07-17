#pragma once
#include "LevelLoader.h"
#include "Obj.h"
#include "Collision.h"
#include "EventBlock.h"
#include "GoalSystem.h"
#include "Goal.h"
#include "Star.h"
#include "CollideManager.h"

class ColObj3d : public Block
{
public:
	Obj3d* collideObj = nullptr;
};

class ColEventObj : public Obj3d
{
public:
	EventBlock* collideObj = nullptr;
};

class Stage
{
public:
	void ChangeLevel(LevelData& data);

	void Update();
	void Draw();

	void DrawSprite();

	std::string GetNowStageHandle();

	static Stage* Get()
	{
		static Stage instance;
		return &instance;
	}

	//���f���̔z��
	std::list<Obj3d> mObj3ds;

	//�����蔻�胂�f���z��
	std::list<ColObj3d> mColObj3ds;

	//�����蔻��z��
	std::list<Cube> mColCubes;

	//�C�x���g�I�u�W�F�N�g�z��
	std::list<std::unique_ptr<EventBlock>> mEventObjects;

	//�S�[���I�u�W�F�N�g�z��
	std::list<std::unique_ptr<Goal>> mGoals;

	//�R���C�_�[��`�悷�邩
	bool mShowCollider = false;

	//���f����`�悷�邩(�R���C�_�[�`�掞�Ɏז��ɂȂ�̂�)
	bool mShowModel = true;

	GoalSystem goalSystem;

private:
	Stage(){};
	~Stage(){};

	//�ʏ�̃I�u�W�F�N�g�z�u
	void NormalObjectSet(const LevelData::ObjectData& data);

	//�����蔻��z�u
	void CollisionSet(const LevelData::ObjectData& data);

	//�C�x���g�I�u�W�F�N�g(�u���b�N�u������)
	void EvenyObjectSet(const LevelData::ObjectData& data);

	//�X�e�[�W�؂�ւ��̍X�V
	void ChangeUpdate();

	void DrawModel();
	void DrawCollider();

	std::string currentHandle = "";

	LevelData* currentData = nullptr;
};

