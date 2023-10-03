#include "Stage.h"
#include "Model.h"
#include "MathF.h"
#include "EnemyManager.h"
#include "Player.h"
#include "WarpBlock.h"
#include "ClearDrawScreen.h"
#include "Star.h"
#include "SceneChange.h"
#include "Cannon.h"
#include <sstream>
#include "Bombking.h"
#include "Coin.h"
#include "Dokan.h"
#include "MoveBlock.h"
#include <fstream>
#include <sstream>
#include "StageTitleUI.h"
#include "EventManager.h"

#include "Clear1.h"
#include "GoalCamChange.h"
#include <GameUIManager.h>

void StageChanger::LoadResource()
{
	StageChanger::Get()->goalSystem.LoadResource();
	Dokan::LoadResource();
}

void StageChanger::ChangeLevel(LevelData& data)
{
	if (SceneChange::GetRun() == false)
	{
		//�V�[���؂�ւ����J�n
		SceneChange::Get()->Start();

		//�X�e�[�W�f�[�^��ۑ�
		currentData = &data;
	}
}

void StageChanger::Initialize(LevelData& data)
{
	//�X�e�[�W�f�[�^��ۑ�
	currentData = &data;

	//�X�e�[�W����
	ChangeUpdate();
}

void StageChanger::Update()
{
	//�V�[���؂�ւ����Ó]�܂œ��B������
	if (SceneChange::Get()->IsBlackOut())
	{
		//�؂�ւ��J�n(��������)
		ChangeUpdate();

		//�؂�ւ����I�������Ó]������
		SceneChange::Get()->Open();
	}

	//�X�e�[�W���ƂɎ��s�����C�x���g������΂����Ŏ��s
	if (GetNowStageHandle() == "stage_stageselect" &&
		LevelLoader::Get()->GetData("stage_mountain")->isClear &&
		EventManager::Get()->GetEvent("next_1")->get()->isExecuted == false)
	{
		EventManager::Get()->Start("next_1");
	}

	for (auto& obj : mEntitys)
	{
		obj.Update(*Camera::sCamera);
		obj.box.Update(*Camera::sCamera);
	}
	for (auto& obj : mEventObjects)
	{
		obj->Update();
	}

	for (auto& obj : CollideManager::Get()->allCols)
	{
		obj->Update(*Camera::sCamera);
	}
	for (auto& obj : mGoals)
	{
		obj->Update();
	}

	seaObject->Update();
	goalSystem.Update();
}

void StageChanger::Draw()
{
	//���ɂ���ă}�e���A���`��ƃe�N�X�`���`�悪���݂��Ă�̂�
	//��������@������ĂȂ��̂ō��
	BasicObjectPreDraw(PipelineManager::GetPipeLine("Toon"));
	DrawCollider();

	DrawModel();
}

void StageChanger::DrawSprite()
{
	goalSystem.Draw();
}

void StageChanger::Reload()
{
	std::string loadfile = "Scene/";
	loadfile += StageChanger::Get()->GetNowStageHandle();
	LevelLoader::Get()->Load(loadfile, StageChanger::Get()->GetNowStageHandle(), StageChanger::Get()->currentData->mStageNum);

	StageChanger::Get()->ChangeLevel(*LevelLoader::Get()->GetData(StageChanger::Get()->GetNowStageHandle()));
}

std::string StageChanger::GetNowStageHandle()
{
	return currentHandle;
}

LevelData* StageChanger::GetNowStageData()
{
	return LevelLoader::Get()->GetData(GetNowStageHandle());
}

void StageChanger::Reset()
{
	//�����Ă����̂��폜
	mEntitys.clear();
	mEventObjects.clear();
	mGoals.clear();
	CollideManager::Get()->allCols.clear();
	mCannonPoints.clear();
	mMoveBlockEndPoints.clear();
	EnemyManager::Get()->enemyList.clear();

	Player::Get()->Register();

	mTempStarSaves.clear();

	IDdCube::ResetID();

	EventManager::Get()->Clear();
}

void StageChanger::NormalObjectSet(const LevelData::ObjectData& data)
{
	mEntitys.emplace_back();
	mEntitys.back().Initialize();
	mEntitys.back().SetTag(TagTable::Block);

	//�R���W�����ړI�Ŕz�u�����Ȃ�I�u�W�F�N�g�z�u���s��Ȃ�
	if (data.fileName == "collision")
	{
		mEntitys.back().SetTag(TagTable::NoDraw);
		return;
	}

	//�A�E�g���C���ݒ�
	mEntitys.back().SetOutLineState({ 0,0,0,1.0f }, 0.05f);

	//���f���ݒ�
	//�t�@�C���l�[�����ݒ肳��Ă�Ȃ炻���
	if (data.fileName != "")
	{
		//�ǂݍ��݂��ĂȂ��Ȃ�ǂݍ��݂��s��
		if (ModelManager::GetModel(data.fileName) == nullptr)
		{
			ModelManager::LoadModel(data.fileName, data.fileName,true);
		}
		mEntitys.back().SetModel(ModelManager::GetModel(data.fileName));
	}
	//�Ȃ��Ȃ�l�p���f�t�H�Őݒ�
	else
	{
		mEntitys.back().SetModel(ModelManager::GetModel("Cube"));
	}

	//�o�O��Ȃ��悤�ɔ��e�N�X�`��������
	mEntitys.back().SetTexture(TextureManager::Get()->GetTexture("white"));
	
	if (data.textureName != "")
	{
		//�w��e�N�X�`��������Ȃ炻�������g��
		mEntitys.back().SetTexture(TextureManager::Get()->GetTexture(data.textureName));
		mEntitys.back().isTexDraw = true;
	}

	//�^�C�����O�̐ݒ�
	mEntitys.back().mTiling = data.tiling;
	
	//�I�u�W�F�N�g�̔z�u
	LevelDataExchanger::SetObjectData(mEntitys.back(), data);

	//�����蔻����쐬
	if (data.collider.have)
	{
		CollisionSet(data);
	}
}

void StageChanger::CollisionSet(const LevelData::ObjectData& data)
{
	//�����蔻���\������I�u�W�F�N�g
	mEntitys.back().box.Initialize();

	//�G���e�B�e�B���X�g�ŎQ�Ƃ��ꂽ���Ȃ��̂ŁA�R���W�����̃^�O��t����
	mEntitys.back().SetTag(TagTable::Collsion);
	
	mEntitys.back().box.SetModel(ModelManager::GetModel("BlankCube"));
	mEntitys.back().box.SetTexture(TextureManager::Get()->GetTexture("white"));

	mEntitys.back().box.position = data.translation + data.collider.center;
	mEntitys.back().box.scale = {
		data.scaling.x * data.collider.size.x,
		data.scaling.y * data.collider.size.y,
		data.scaling.z * data.collider.size.z
	};
	mEntitys.back().box.rotation = {
		MathF::AngleConvRad(data.rotation.x),
		MathF::AngleConvRad(data.rotation.y),
		MathF::AngleConvRad(data.rotation.z)
	};

	mEntitys.back().box.cubecol.position = mEntitys.back().box.position;
	mEntitys.back().box.cubecol.scale = mEntitys.back().box.scale;
	//�����蔻�肾���}�l�[�W���[�ɓo�^
	mEntitys.back().Register();
}

void StageChanger::CollisionSetEvent(const LevelData::ObjectData& data)
{
	//�����蔻���\������I�u�W�F�N�g
	mEventObjects.back()->box.Initialize();

	//�R���W�����I�����[�`��Ŏg�����߁A�R���W�����̃^�O��t����
	mEventObjects.back()->SetTag(TagTable::Collsion);

	mEventObjects.back()->box.SetModel(ModelManager::GetModel("BlankCube"));
	mEventObjects.back()->box.SetTexture(TextureManager::Get()->GetTexture("white"));

	mEventObjects.back()->box.position = data.translation + data.collider.center;
	mEventObjects.back()->box.scale = {
		data.scaling.x * data.collider.size.x,
		data.scaling.y * data.collider.size.y,
		data.scaling.z * data.collider.size.z
	};
	mEventObjects.back()->box.rotation = {
		MathF::AngleConvRad(data.rotation.x),
		MathF::AngleConvRad(data.rotation.y),
		MathF::AngleConvRad(data.rotation.z)
	};

	mEventObjects.back()->box.cubecol.position = mEntitys.back().box.position;
	mEventObjects.back()->box.cubecol.scale = mEntitys.back().box.scale;
	//�����蔻�肾���}�l�[�W���[�ɓo�^
	mEventObjects.back()->Register();
}

void StageChanger::EvenyObjectSet(const LevelData::ObjectData& data)
{
	std::string tFilename = "";
	if (data.fileName != "")
	{
		//�ǂݍ��݂��ĂȂ��Ȃ�ǂݍ��݂��s��
		if (ModelManager::GetModel(data.fileName) == nullptr)
		{
			ModelManager::LoadModel(data.fileName, data.fileName, true);
		}
		tFilename = data.fileName;
	}
	
	//stage �̕����񂪊܂܂�Ă�Ȃ�
	if (data.eventtrigerName.find("stage") != std::string::npos)
	{
		mEventObjects.emplace_back();
		mEventObjects.back() = std::make_unique<WarpBlock>();
		mEventObjects.back()->Initialize();
		//�t�@�C���l�[��������Ȃ�
		if (tFilename != "")
		{
			mEventObjects.back()->SetModel(ModelManager::GetModel(tFilename));
		}

		mEventObjects.back()->SetOutLineState({ 1,0,0,1.0f }, 0.05f);

		mEventObjects.back()->trigerName = data.eventtrigerName;
		//�o�O��Ȃ��悤�ɔ��e�N�X�`��������
		mEventObjects.back()->SetTexture(TextureManager::Get()->GetTexture("white"));

		//�I�u�W�F�N�g�̔z�u
		LevelDataExchanger::SetObjectData(*mEventObjects.back(), data);

		return;
	}
	//goal �̕����񂪊܂܂�Ă�Ȃ�
	if (data.eventtrigerName == "goal")
	{
		mGoals.emplace_back();
		mGoals.back() = std::make_unique<Goal>();
		mGoals.back()->Initialize();

		mGoals.back()->trigerName = data.eventtrigerName;

		mGoals.back()->SetOutLineState({ 1,0,0,1.0f }, 0.05f);

		//�I�u�W�F�N�g�̔z�u
		LevelDataExchanger::SetObjectData(*mGoals.back(), data);

		return;
	}

	//star �̕����񂪊��S��v����Ȃ�
	if (data.eventtrigerName == "star")
	{
		mEventObjects.emplace_back();
		mEventObjects.back() = std::make_unique<Star>();
		mEventObjects.back()->Initialize();
		//�t�@�C���l�[��������Ȃ�
		if (tFilename != "")
		{
			mEventObjects.back()->SetModel(ModelManager::GetModel(tFilename));
		}

		mEventObjects.back()->SetOutLineState({ 0,0,0,1.0f }, 0.1f);

		mEventObjects.back()->trigerName = data.eventtrigerName;
		mEventObjects.back()->box.CreateCol(
			mEventObjects.back()->position,
			mEventObjects.back()->scale
		);
		
		//�����蔻����쐬
		//starManager�ɂ킯��Ȉ�ӂɒ�`�o���˂�����I�I�I�I�I!!!!!!1
		if (data.collider.have)
		{
			//�����蔻���\������I�u�W�F�N�g
			mEventObjects.back()->box.Initialize();

			//�R���W�����I�����[�`��Ŏg�����߁A�R���W�����̃^�O��t����
			mEventObjects.back()->SetTag(TagTable::Collsion);
			//�u���b�N�̃^�O���O��
			mEventObjects.back()->DeleteTag(TagTable::Block);

			mEventObjects.back()->box.SetModel(ModelManager::GetModel("BlankCube"));
			mEventObjects.back()->box.SetTexture(TextureManager::Get()->GetTexture("white"));

			mEventObjects.back()->box.position = data.translation + data.collider.center;
			mEventObjects.back()->box.scale = {
				data.scaling.x * data.collider.size.x,
				data.scaling.y * data.collider.size.y,
				data.scaling.z * data.collider.size.z
			};
			mEventObjects.back()->box.rotation = {
				MathF::AngleConvRad(data.rotation.x),
				MathF::AngleConvRad(data.rotation.y),
				MathF::AngleConvRad(data.rotation.z)
			};

			//����Entitys������������Ă��Ă邯�Ǎ����Ă�̂���
			mEventObjects.back()->box.cubecol.position = mEventObjects.back()->box.position;
			mEventObjects.back()->box.cubecol.scale = mEventObjects.back()->box.scale;
			//�����蔻�肾���}�l�[�W���[�ɓo�^
			mEventObjects.back()->Register();
		}

		//�I�u�W�F�N�g�̔z�u
		LevelDataExchanger::SetObjectData(*mEventObjects.back(), data);

		Star* star = static_cast<Star*>(mEventObjects.back().get());

		star->id = atoi(data.setObjectName.c_str());

		mTempStarSaves.push_back(star);

		return;
	}
	//coin �̕����񂪊��S��v����Ȃ�
	if (data.eventtrigerName == "coin")
	{
		mEventObjects.emplace_back();
		mEventObjects.back() = std::make_unique<Coin>();
		mEventObjects.back()->Initialize();
		//�t�@�C���l�[��������Ȃ�
		if (tFilename != "")
		{
			mEventObjects.back()->SetModel(ModelManager::GetModel(tFilename));
		}

		mEventObjects.back()->SetOutLineState({ 0,0,0,1.0f }, 0.1f);

		mEventObjects.back()->trigerName = data.eventtrigerName;

		//�����蔻����쐬
		if (data.collider.have)
		{
			CollisionSetEvent(data);
		}

		//�I�u�W�F�N�g�̔z�u
		LevelDataExchanger::SetObjectData(*mEventObjects.back(), data);

		return;
	}
	//Cannon �̕����񂪊܂܂�Ă�Ȃ�
	if (data.eventtrigerName.find("Cannon") != std::string::npos)
	{
		if (data.eventtrigerName.find("start") != std::string::npos)
		{
			std::stringstream ss;

			mEventObjects.emplace_back();
			mEventObjects.back() = std::make_unique<Cannon>();
			mEventObjects.back()->Initialize();
			
			std::vector<std::string> split = Util::SplitString(data.eventtrigerName, "_");

			//�C�x���g�g���K�[����stringstream�ɑ��
			for (auto str : split)
			{
				//�������������o��
				if(Util::IsNumber(str)) ss << str;
			}

			Cannon* cannon = static_cast<Cannon*>(mEventObjects.back().get());
			
			//int32_t�^�ɏo�͂���
			//���̍ہA�����񂩂琔�l�݂̂��o�͂����
			ss >> cannon->id;
			cannon->startPos = data.translation;

			//�I�u�W�F�N�g�̔z�u
			LevelDataExchanger::SetObjectData(*mEventObjects.back(), data);
		}
		//�S�ẴI�u�W�F�N�g�z�u��A����_�Ƒ�C�݂̂Ŕ�r���Ȃ������߁A
		//�l���ꎞ�I�ɕۑ����Ă���
		if (data.eventtrigerName.find("inter") != std::string::npos)
		{	
			CannonPoint point = { data.eventtrigerName, data.translation };
			mCannonPoints.push_back(point);
		}
		if (data.eventtrigerName.find("end") != std::string::npos)
		{
			CannonPoint point = { data.eventtrigerName, data.translation };
			mCannonPoints.push_back(point);
		}

		return;
	}
}

void StageChanger::ChangeUpdate()
{
	//�n���h�����X�e�[�W�ɕۑ�
	currentHandle = currentData->mHandle;

	Reset();

	for (auto objectData = currentData->mObjects.begin(); objectData != currentData->mObjects.end(); objectData++)
	{
		//�J�����̔z�u�Ȃ�
		if (objectData->setObjectName == "eventcamera")
		{
			EventCamData camdata;
			camdata.pos = objectData->translation;
			camdata.rotation = objectData->rotation;
			if (objectData->eventtrigerName == "next_1")
			{
				EventManager::Get()->Register<Clear1>(camdata, objectData->eventtrigerName);
			}
			if (objectData->eventtrigerName == "goalCamera")
			{
				EventManager::Get()->Register<GoalCamChange>(camdata, objectData->eventtrigerName);
			}
			if (Util::CheckString(objectData->eventtrigerName,"moveCamera"))
			{

			}
		}

		//�C�̔z�u�Ȃ�
		if (objectData->setObjectName == "sea")
		{
			seaObject = std::make_unique<Sea>();
			seaObject->LoadResource();
			seaObject->Initialize();
			seaObject->SetInfo(
				objectData->translation,
				objectData->scaling,
				objectData->tiling);

			continue;
		}

		//�v���C���[�̔z�u�Ȃ�
		if (objectData->setObjectName == "player")
		{
			if (playerData.dokanPriority == false)
			{
				playerData.data = *objectData;
				Player::Get()->mDokanApparrance = false;
			}
			
			continue;
		}

		//�����͌�ŁAcannon�̎��݂����ɏ������`����if����ɂ܂Ƃ߂���
		//�G�l�~�[�̔z�u�Ȃ�
		if (objectData->setObjectName == "enemy")
		{
			EnemyManager::Get()->Load(*objectData);
			
			if (objectData->collider.have)
			{
				//�����蔻���\������I�u�W�F�N�g
				EnemyManager::Get()->enemyList.back()->box.Initialize();

				//�G���e�B�e�B���X�g�ŎQ�Ƃ��ꂽ���Ȃ��̂ŁA�R���W�����̃^�O��t����
				EnemyManager::Get()->enemyList.back()->SetTag(TagTable::Collsion);

				EnemyManager::Get()->enemyList.back()->box.SetModel(ModelManager::GetModel("BlankCube"));
				EnemyManager::Get()->enemyList.back()->box.SetTexture(TextureManager::Get()->GetTexture("white"));

				//���S�ʒu�����炷����ۑ�
				EnemyManager::Get()->enemyList.back()->saveColCenter = objectData->collider.center;

				EnemyManager::Get()->enemyList.back()->box.position = objectData->translation + EnemyManager::Get()->enemyList.back()->saveColCenter;
				EnemyManager::Get()->enemyList.back()->box.scale = {
					objectData->scaling.x * objectData->collider.size.x,
					objectData->scaling.y * objectData->collider.size.y,
					objectData->scaling.z * objectData->collider.size.z
				};
				EnemyManager::Get()->enemyList.back()->box.rotation = {
					MathF::AngleConvRad(objectData->rotation.x),
					MathF::AngleConvRad(objectData->rotation.y),
					MathF::AngleConvRad(objectData->rotation.z)
				};

				EnemyManager::Get()->enemyList.back()->box.cubecol.position = EnemyManager::Get()->enemyList.back()->box.position;
				EnemyManager::Get()->enemyList.back()->box.cubecol.scale = EnemyManager::Get()->enemyList.back()->box.scale;
				//�����蔻�肾���}�l�[�W���[�ɓo�^
				EnemyManager::Get()->enemyList.back()->Register();
			}

			continue;
		}
		//�{�X�̔z�u�Ȃ�
		if (objectData->setObjectName == "bombking")
		{
			EnemyManager::Get()->enemyList.emplace_back();
			EnemyManager::Get()->enemyList.back() = std::make_unique<Bombking>();

			//�ǂݍ��݂��ĂȂ��Ȃ�ǂݍ��݂��s��
			if (ModelManager::GetModel(objectData->fileName) == nullptr)
			{
				ModelManager::LoadModel(objectData->fileName, objectData->fileName, true);
			}

			//���f���Ƃ��ݒ肷��
			EnemyManager::Get()->enemyList.back()->Initialize();

			//position�Ƃ���ݒ�
			LevelDataExchanger::SetObjectData(*EnemyManager::Get()->enemyList.back(), *objectData);
			
			if(objectData->collider.have)
			{
				//�����蔻���\������I�u�W�F�N�g
				EnemyManager::Get()->enemyList.back()->box.Initialize();

				//�G���e�B�e�B���X�g�ŎQ�Ƃ��ꂽ���Ȃ��̂ŁA�R���W�����̃^�O��t����
				EnemyManager::Get()->enemyList.back()->SetTag(TagTable::Collsion);

				EnemyManager::Get()->enemyList.back()->box.SetModel(ModelManager::GetModel("BlankCube"));
				EnemyManager::Get()->enemyList.back()->box.SetTexture(TextureManager::Get()->GetTexture("white"));

				//���S�ʒu�����炷����ۑ�
				EnemyManager::Get()->enemyList.back()->saveColCenter = objectData->collider.center;

				EnemyManager::Get()->enemyList.back()->box.position = objectData->translation + EnemyManager::Get()->enemyList.back()->saveColCenter;
				EnemyManager::Get()->enemyList.back()->box.scale = {
					objectData->scaling.x * objectData->collider.size.x,
					objectData->scaling.y * objectData->collider.size.y,
					objectData->scaling.z * objectData->collider.size.z
				};
				EnemyManager::Get()->enemyList.back()->box.rotation = {
					MathF::AngleConvRad(objectData->rotation.x),
					MathF::AngleConvRad(objectData->rotation.y),
					MathF::AngleConvRad(objectData->rotation.z)
				};

				EnemyManager::Get()->enemyList.back()->box.cubecol.position = EnemyManager::Get()->enemyList.back()->box.position;
				EnemyManager::Get()->enemyList.back()->box.cubecol.scale = EnemyManager::Get()->enemyList.back()->box.scale;
				//�����蔻�肾���}�l�[�W���[�ɓo�^
				EnemyManager::Get()->enemyList.back()->Register();
			}

			continue;
		}

		if (objectData->setObjectName.find("moveBlock") != std::string::npos)
		{
			std::string saveID = "";
			//eventtrigerName�𕪉�����
			std::vector<std::string> split = Util::SplitString(objectData->eventtrigerName, "_");

			for (auto str : split)
			{
				//�������������o��
				if (Util::IsNumber(str))
				{
					saveID = str;
				}
			}

			//end���t���Ă���Ȃ�ID�����ō��W�����Ĕ�΂�
			if (objectData->eventtrigerName.find("end") != std::string::npos)
			{
				mMoveBlockEndPoints.emplace_back();

				mMoveBlockEndPoints.back().key = saveID;
				mMoveBlockEndPoints.back().points = objectData->translation;

				continue;
			}

			//end�łȂ��Ȃ�start�̂͂��Ȃ̂ŁA�I�u�W�F�N�g�𐶐�
			mEventObjects.emplace_back();
			mEventObjects.back() = std::make_unique<MoveBlock>();
		
			MoveBlock* mb = dynamic_cast<MoveBlock*>(mEventObjects.back().get());
			
			//�����n�_�Ȃ�ʒu����
			if (objectData->eventtrigerName.find("start") != std::string::npos)
			{
				mb->id = saveID;
				mb->startpos = objectData->translation;
			}

			mEventObjects.back()->Initialize();
			mEventObjects.back()->SetOutLineState({ 0,0,0,1.0f }, 0.1f);

			if (objectData->fileName != "")
			{
				//�ǂݍ��݂��ĂȂ��Ȃ�ǂݍ��݂��s��
				if (ModelManager::GetModel(objectData->fileName) == nullptr)
				{
					ModelManager::LoadModel(objectData->fileName, objectData->fileName, true);
				}
				mEventObjects.back()->SetModel(ModelManager::GetModel(objectData->fileName));
			}

			//�I�u�W�F�N�g�̔z�u
			LevelDataExchanger::SetObjectData(*mEventObjects.back(), *objectData);

			//�����蔻����쐬
			if (objectData->collider.have)
			{
				CollisionSetEvent(*objectData);
			}

			continue;
		}

		//�y�ǂ�z�u
		if (objectData->setObjectName.find("dokan") != std::string::npos)
		{
			mEventObjects.emplace_back();
			mEventObjects.back() = std::make_unique<Dokan>();

			mEventObjects.back()->Initialize();

			mEventObjects.back()->trigerName = objectData->eventtrigerName;

			mEventObjects.back()->SetOutLineState({ 0,0,0,1 }, 0.05f);

			//�I�u�W�F�N�g�̔z�u
			LevelDataExchanger::SetObjectData(*mEventObjects.back(), *objectData);

			if (objectData->collider.have)
			{
				//�����蔻���\������I�u�W�F�N�g
				mEventObjects.back()->box.Initialize();

				//�R���W�����I�����[�`��Ŏg�����߁A�R���W�����̃^�O��t����
				mEventObjects.back()->SetTag(TagTable::Collsion);

				mEventObjects.back()->box.SetModel(ModelManager::GetModel("BlankCube"));
				mEventObjects.back()->box.SetTexture(TextureManager::Get()->GetTexture("white"));

				mEventObjects.back()->box.position = objectData->translation + objectData->collider.center;
				mEventObjects.back()->box.scale = {
					objectData->scaling.x * objectData->collider.size.x,
					objectData->scaling.y * objectData->collider.size.y,
					objectData->scaling.z * objectData->collider.size.z
				};
				mEventObjects.back()->box.rotation = {
					MathF::AngleConvRad(objectData->rotation.x),
					MathF::AngleConvRad(objectData->rotation.y),
					MathF::AngleConvRad(objectData->rotation.z)
				};

				mEventObjects.back()->box.cubecol.position = mEventObjects.back()->box.position;
				mEventObjects.back()->box.cubecol.scale = mEventObjects.back()->box.scale;
			}

			//���g�̏���ݒ肷��
			std::vector<std::string> split = Util::SplitString(objectData->eventtrigerName, "_");

			Dokan* dokan = dynamic_cast<Dokan*>(mEventObjects.back().get());
			for (auto str : split)
			{
				//�ړ���̓y��ID�����o��
				if (Util::IsNumber(str))
				{
					dokan->nextDokanInfo.id = atoi(str.c_str());
				}
				//�ړ���̃X�e�[�W�������o��
				else
				{
					dokan->nextDokanInfo.stageName = str;
				}
			}

			split = Util::SplitString(objectData->setObjectName, "_");
			for (auto str : split)
			{
				//���g�̓y��ID�����o��
				if (Util::IsNumber(str))
				{
					dokan->dokanInfo.id = atoi(str.c_str());
				}
			}
			//���݂̃X�e�[�W�n���h��(�X�e�[�W��)��ۑ����āA���g�̏��Ƃ���
			split = Util::SplitString(currentHandle, "_");

			for (auto str : split)
			{
				//�����񂩂犥����"stage"����菜�����҂�ۑ�
				if (str != "stage")
				{
					dokan->dokanInfo.stageName = str;
				}
			}

			//�ړ��O�̓y�ǂ������Ă������ƈ�v����y�ǂ���������
			if (saveNextDokanInfo.stageName == dokan->dokanInfo.stageName &&
				saveNextDokanInfo.id == dokan->dokanInfo.id)
			{
				//�v���C���[�̏����L�^
				playerData.data = *objectData;
				//�D��t���O�𗧂Ă�
				playerData.dokanPriority = true;

				//�X�e�[�W����UI�ɋL�^
				GameUIManager::Get()->GetStageTitleUI()->ChangeHandle(StageChanger::Get()->currentData->mStageNum);
			}

			continue;
		}

		if (objectData->setObjectName == "boardpicture")
		{
			//���̂܂܃��f���̔z�u
			NormalObjectSet(*objectData);

			mEntitys.back().SetModel(ModelManager::GetModel("plate"));
			mEntitys.back().rotation += { 
				0,
				MathF::AngleConvRad(-180.f),
				MathF::AngleConvRad(-90.f),
			};

			//�C�x���g�g���K�[�ɋL�ڂ��ꂽ�e�N�X�`���������g�̃e�N�X�`���Ƃ��ē\��
			//�n���h�����Ȃ��Ȃ�쐬
			if (TextureManager::GetTexture(objectData->eventtrigerName) == nullptr)
			{
				std::string filename = "Resources\\";
				filename = filename + objectData->eventtrigerName + ".png";
				TextureManager::Load(filename, objectData->eventtrigerName);
			}
			mEntitys.back().SetTexture(TextureManager::GetTexture(objectData->eventtrigerName));

			//�����蔻����쐬
			if (objectData->collider.have)
			{
				CollisionSet(*objectData);
			}

			continue;
		}

		//�C�x���g�I�u�W�F�N�g�Ȃ�ݒu
		if (objectData->eventtrigerName != "")
		{
			//���ł���ɕ��ނ킯���Ĕz�u���Ă���
			//EventBlock �����N���X�ɁAHitEffect�̒��g��ς����N���X�Ŏ������Ă���
			EvenyObjectSet(*objectData);

			continue;
		}

		//---�������O��continue�Y���ƁA���f����ǂݍ��񂶂���ăo�O��\����

		//�Ȃɂ�����������
		{
			//�����܂ŉ��������ĂȂ���Βn�`�̔z�u�Ƃ��Ĉ���
			//�{����Block�^�ɕϊ����ē��ꂽ�����A�|�C���^�ŕێ�����`�ɂȂ��ĂȂ��̂�Block�^�ɂł��Ȃ�
			//�Ȃ̂�Tag�������Block������`�ɂ��ĉ��Ƃ����Ă���

			//���̂܂܃��f���̔z�u
			NormalObjectSet(*objectData);

			continue;
		}
	}

	//�S�ẴC�x���g�I�u�W�F�N�g����������
	for (auto itr = mEventObjects.begin(); itr != mEventObjects.end(); itr++)
	{
		//Cannon�^�ɃL���X�g
		//dynamic_cast�̎d�l�ŁACannon�łȂ����nullptr�Ɣ��肳���̂�
		Cannon* cannon = dynamic_cast<Cannon*>(itr->get());
		//�قȂ����ꍇ����
		if (cannon != nullptr)
		{
			//�ʂ����ꍇ�́A���O�ɕۊǂ��Ă���������_�̈ꗗ����
			for (auto itr = mCannonPoints.begin(); itr != mCannonPoints.end(); itr++)
			{
				//���ԓ_��
				if (itr->key.find("inter") != std::string::npos)
				{
					//id����v�����ꍇ�͓����
					if (itr->key.find(cannon->id) != std::string::npos)
					{
						cannon->interPos = itr->points;
					}
				}
				//�ŏI�_�𔻕ʂ���
				if (itr->key.find("end") != std::string::npos)
				{
					//id����v�����ꍇ�͓����
					if (itr->key.find(cannon->id) != std::string::npos)
					{
						cannon->endPos = itr->points;
					}
				}
			}
		}

		MoveBlock* moveBlock = dynamic_cast<MoveBlock*>(itr->get());

		if (moveBlock != nullptr)
		{
			//�ʂ����ꍇ�́A���O�ɕۊǂ��Ă���������_�̈ꗗ����
			for (auto itr = mMoveBlockEndPoints.begin(); itr != mMoveBlockEndPoints.end(); itr++)
			{
				//id����v�����ꍇ�͓����
				if (itr->key.find(moveBlock->id) != std::string::npos)
				{
					moveBlock->endpos = itr->points;
				}
			}
		}

		//TODO:���O�ɕۊǂ����f�[�^�Q����������ہA���������S����(���`�T��)���Ă���̂�
		//�f�[�^�̑��ʂ�������Ə������Ԃ��̂тĂ��܂� �A�z
		//�T������ς��Ȃ����j�ŉ��P����Ȃ�A���łɑS�Ẵf�[�^���������Ă���f�[�^��
		//�X�L�b�v����Ȃǂ̏������K�v���낤
	}

	SetPlayer(playerData.data);
	//�y�ǂ���̔z�u�łȂ����
	if (playerData.dokanPriority == false)
	{
		//���݂̃X�e�[�W�n���h��(�X�e�[�W��)��ۑ����āA���g�̏��Ƃ���
	 	std::vector<std::string> split = Util::SplitString(currentHandle, "_");

		//�f�[�^�ɏ���������������
		for (auto str : split)
		{
			//�����񂩂犥����"stage"����菜�����҂�ۑ�
			if (str != "stage")
			{
				saveNextDokanInfo.stageName = str;
			}
		}

		saveNextDokanInfo.id = 0;
		Player::Get()->mDokanApparrance = false;
	}
}

void StageChanger::SetPlayer(const LevelData::ObjectData& data)
{
	Player::Get()->Reset();
	Player::Get()->preMove = data.translation;
	Player::Get()->position = data.translation;
	Player::Get()->rotation = data.rotation;

	Player::Get()->box.position = Player::Get()->position;
	Player::Get()->box.scale = Player::Get()->scale;
	Player::Get()->box.cubecol.position = Player::Get()->position;
	Player::Get()->box.cubecol.scale = Player::Get()->scale;

	Player::Get()->mDokanApparrance = true;
}

void StageChanger::DrawModel()
{
	if (mShowModel == false) return;
	for (auto& obj : mEntitys)
	{
		//�R���W�����p�ɔz�u�����I�u�W�F�N�g�Ȃ�X�L�b�v
		if (obj.CheckTag(TagTable::NoDraw))continue;
		
		BasicObjectPreDraw(PipelineManager::GetPipeLine("OutLine"), false);
		obj.DrawOutLine();
		//�A���t�@��1�����ɂȂ�Ȃ瓧���p�`��p�C�v���C���ɐ؂�ւ���
		if (obj.color_.w < 1.0f)
		{
			BasicObjectPreDraw(PipelineManager::GetPipeLine("GroundToonNDW"));
		}
		else
		{
			BasicObjectPreDraw(PipelineManager::GetPipeLine("GroundToon"));
		}
		if (obj.isTexDraw)
		{
			obj.Draw();
		}
		else
		{
			obj.DrawMaterial();
		}
	}
	for (auto& obj : mEventObjects)
	{
		BasicObjectPreDraw(PipelineManager::GetPipeLine("OutLine"),false);
		obj->DrawOutLine();
		//�A���t�@��1�����ɂȂ�Ȃ瓧���p�`��p�C�v���C���ɐ؂�ւ���
		if (obj->color_.w < 1.0f)
		{
			BasicObjectPreDraw(PipelineManager::GetPipeLine("GroundToonNDW"));
		}
		else
		{
			BasicObjectPreDraw(PipelineManager::GetPipeLine("GroundToon"));
		}

		obj->Draw();
	}

	for (auto& obj : mGoals)
	{
		BasicObjectPreDraw(PipelineManager::GetPipeLine("OutLine"), false);
		obj->DrawOutLine();
		//�A���t�@��1�����ɂȂ�Ȃ瓧���p�`��p�C�v���C���ɐ؂�ւ���
		if (obj->color_.w < 1.0f)
		{
			BasicObjectPreDraw(PipelineManager::GetPipeLine("ToonNDW"));
		}
		else
		{
			BasicObjectPreDraw(PipelineManager::GetPipeLine("Toon"));
		}
		obj->Draw();
	}

	BasicObjectPreDraw(PipelineManager::GetPipeLine("GroundToon"));
	seaObject->Draw();
}

void StageChanger::DrawCollider()
{
	if (mShowCollider == false) return;
	for (auto& obj : mEntitys)
	{
		if (!obj.CheckTag(TagTable::Collsion))continue;

		obj.box.Draw();
	}
	for (auto& obj : mEventObjects)
	{
		if (!obj->CheckTag(TagTable::Collsion))continue;

		obj->box.Draw();
	}
}

void StageChanger::ResetRevise(int32_t stageNumber, int32_t starID, int32_t starnum)
{
	//�t�@�C���o�͏���
	std::ofstream writing_file;

	std::string filename = "";
	filename = "./Resources/data/star/";

	std::ostringstream oss;
	oss << stageNumber;

	filename = filename + "stage_" + oss.str() + ".txt";

	writing_file.open(filename, std::ios::out);

	for (int i = 0; i < starnum; i++)
	{
		//�擾�󋵂�0(���擾)�ɏ���������
		if (starID == i)
		{
			int h = 0;
			writing_file << h;
			writing_file << "," << std::endl;
		}
	}

	writing_file.close();
}

void StageChanger::CorrectedRevise(int32_t stageNumber, int32_t starID, int32_t starnum)
{
	//�t�@�C���o�͏���
	std::ofstream writing_file;

	std::string filename = "";
	filename = "./Resources/data/star/";

	std::ostringstream oss;
	oss << stageNumber;

	filename = filename + "stage_" +  oss.str() + ".txt";

	writing_file.open(filename, std::ios::out);

	for (int i = 0; i < starnum; i++)
	{
		int h = 0;
		//�擾�󋵂�1(�擾��)�ɏ���������
		if (starID == i)
		{
			h = 1;
		}
		writing_file << h;
		writing_file << "," << std::endl;
	}

	writing_file.close();
}

bool StageChanger::LoadStarCorrect(int32_t stageNumber, int32_t starID)
{
	//�t�@�C�����͏���
	std::ifstream reading_file;

	std::string filename = "";
	filename = "./Resources/data/star/";

	std::ostringstream oss;
	oss << stageNumber;

	filename = filename + "stage_" + oss.str() + ".txt";

	reading_file.open(filename);

	//�t�@�C���I�[�v�����s���`�F�b�N
	if (reading_file.fail())
	{
		assert(0);
	}

	std::string line;
	int i = 0;
	while (getline(reading_file, line))
	{
		if(i == starID)
		{
			if (line == "0,")
			{
				return 0;
			}
			if (line == "1,")
			{
				return 1;
			}
		}
		i++;
	}

	return 0;
}
