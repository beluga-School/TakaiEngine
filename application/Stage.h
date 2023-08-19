#pragma once
#include "LevelLoader.h"
#include "Obj.h"
#include "Collision.h"
#include "EventBlock.h"
#include "GoalSystem.h"
#include "Goal.h"
#include "Star.h"
#include "CollideManager.h"
#include "Mob.h"
#include "Dokan.h"
#include "Sea.h"

class ColEventObj : public Obj3d
{
public:
	EventBlock* collideObj = nullptr;
};

struct CannonPoint
{
	std::string key = "";
	Vector3 points{};
};

struct MoveBlockPoint
{
	std::string key = "";
	Vector3 points{};
};

class StageChanger
{
public:
	void ChangeLevel(LevelData& data);

	//�����X�e�[�W������
	void Initialize(LevelData& data);
	void Update();
	void Draw();

	void DrawSprite();

	void Reload();

	std::string GetNowStageHandle();

	static StageChanger* Get()
	{
		static StageChanger instance;
		return &instance;
	}

	//���f���̔z��
	//Entity�̃|�C���^�ŕۑ����������֗��Ɏg���邩������Ȃ�
	std::list<Entity> mEntitys;

	//�C�x���g�I�u�W�F�N�g�z��
	std::list<std::unique_ptr<EventBlock>> mEventObjects;

	//�S�[���I�u�W�F�N�g�z��
	std::list<std::unique_ptr<Goal>> mGoals;

	//��C�̐���_���ꎞ�I�ɕۑ�����p�z��
	std::vector<CannonPoint> mCannonPoints;

	//�ړ����̍ŏI�n�_���ꎞ�I�ɕۑ�����z��
	std::vector<MoveBlockPoint> mMoveBlockEndPoints;

	//�X�^�[���ꎞ�I�ɕۑ�����ϐ�
	std::vector<Star*> mTempStarSaves;

	//�C�I�u�W�F�N�g��z�u(������艺�̍��W�ɗ�������MISS����)
	std::unique_ptr<Sea> seaObject;

	//�R���C�_�[��`�悷�邩
	bool mShowCollider = false;

	//���f����`�悷�邩(�R���C�_�[�`�掞�Ɏז��ɂȂ�̂�)
	bool mShowModel = true;

	GoalSystem goalSystem;

	DokanInfo oldDokanInfo;

	//�擾�󋵂�����������
	void ResetRevise(int32_t stageNumber, int32_t starID,int32_t starnum);
	//�擾�󋵂��擾��Ԃɂ���
	void CorrectedRevise(int32_t stageNumber, int32_t starID,int32_t starnum);
	//�擾��Ԃ�ǂݎ���āAbool�ŕԂ�
	bool LoadStarCorrect(int32_t stageNumber, int32_t starID);

private:
	StageChanger(){};
	~StageChanger(){};

	//�X�e�[�W�����[�h�̍ۂɏ�����������
	void Reset();

	//�ʏ�̃I�u�W�F�N�g�z�u
	void NormalObjectSet(const LevelData::ObjectData& data);

	//�����蔻��z�u
	void CollisionSet(const LevelData::ObjectData& data);

	//�C�x���g�I�u�W�F�N�g�p�����蔻��z�u(����mEntity�ňꊇ�Ǘ�������)
	void CollisionSetEvent(const LevelData::ObjectData& data);

	//�C�x���g�I�u�W�F�N�g(�u���b�N�u������)
	void EvenyObjectSet(const LevelData::ObjectData& data);

	//�X�e�[�W�؂�ւ��̍X�V
	void ChangeUpdate();

	void SetPlayer(const LevelData::ObjectData& data);

	void DrawModel();
	void DrawCollider();

	std::string currentHandle = "";

	LevelData* currentData = nullptr;

	GUI hoge = { "hoge" };
};

