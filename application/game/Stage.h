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
#include "EventCamera.h"
#include <vector>
#include "EventManager.h"

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

struct PlayerData
{
	LevelData::ObjectData data;
	//�y�ǂł̔z�u���������Ă���Ȃ炻������D�悷��悤�ɂ���t���O
	bool dokanPriority = false;
};

enum CameraNumber
{
	Error = -1,
	Target = -2,
};

//�C�x���g�J�����f�[�^���܂Ƃ߂ēǂݍ��ޗp�̃f�[�^�z��
struct LoadCamData
{
	LoadCamData(int32_t eventnumber_, EventCamData camData_) 
	{
		eventnumber = eventnumber_;
		camData.pos = camData_.pos;
		camData.rotation = camData_.rotation;
	};

	int32_t eventnumber = -1;
	
	EventCamData camData{};
};

struct LoadTargetData
{
	LoadTargetData(std::string& eventname_,const Vector3& target_)
	{
		eventname = eventname_;
		target = target_;
	}

	std::string eventname = "";

	Vector3 target = {};
};

class StageChanger
{
public:
	static void LoadResource();

	void ChangeLevel(LevelData& data);

	//�����X�e�[�W������
	void Initialize(LevelData& data);
	void Update();
	void Draw();

	void DrawSprite();

	void Reload();

	std::string GetNowStageHandle();

	//���ݓǂݍ��܂�Ă���X�e�[�W�̃f�[�^��Ԃ�
	LevelData* GetNowStageData();

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

	DokanInfo saveNextDokanInfo;

	//�擾�󋵂�����������
	void ResetRevise(int32_t stageNumber, int32_t starID,int32_t starnum);
	//�擾�󋵂��擾��Ԃɂ���
	void CorrectedRevise(int32_t stageNumber, int32_t starID,int32_t starnum);
	//�擾��Ԃ�ǂݎ���āAbool�ŕԂ�
	bool LoadStarCorrect(int32_t stageNumber, int32_t starID);

	//�ʏ�̃I�u�W�F�N�g�z�u
	void NormalObjectSet(const LevelData::ObjectData& data);

private:
	StageChanger(){};
	~StageChanger(){};

	//�X�e�[�W�����[�h�̍ۂɏ�����������
	void Reset();

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

	void EventNameUniquePush(const std::string& eventname);

	std::string currentHandle = "";

	LevelData* currentData = nullptr;

	PlayerData playerData;

	std::unordered_map<std::string, std::vector<LoadCamData>> loadCamDatas;
	std::vector<LoadTargetData> loadTargetDatas;
	std::list<std::string> eventCameraNames;

	GUI hoge = { "hoge" };

	template <class TEventCamera> void CameraLoader(const LevelData::ObjectData& data, const std::string& eventname)
	{
		EventCamData camdata;
		camdata.pos = data.translation;
		camdata.rotation = data.rotation;

		if (Util::CheckString(data.eventtrigerName, eventname))
		{
			//�����𕪗�
			int32_t number = Util::GetNumber(data.eventtrigerName, "_");
			//�C�x���g���𕪗�
			std::string eventName = Util::GetString(data.eventtrigerName, eventname);

			EventManager::Get()->Register<TEventCamera>(eventName);

			EventNameUniquePush(eventName);

			//�^�[�Q�b�g�ł���Ȃ�
			if (Util::CheckString(data.eventtrigerName, "target"))
			{
				//�^�[�Q�b�g�������ԍ���}��
				loadTargetDatas.emplace_back(eventName, camdata.pos);
				//�c����΂�
				return;
			}

			//�C�x���g���A�ԍ��A�J�����ʒu��ۑ�
			loadCamDatas[eventName].emplace_back(number, camdata);
		}
	}
};

