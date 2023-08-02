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

void Stage::ChangeLevel(LevelData& data)
{
	if (SceneChange::GetRun() == false)
	{
		//�V�[���؂�ւ����J�n
		SceneChange::Get()->Start();

		//�X�e�[�W�f�[�^��ۑ�
		currentData = &data;
	}
}

void Stage::Initialize(LevelData& data)
{
	//�X�e�[�W�f�[�^��ۑ�
	currentData = &data;

	//�X�e�[�W����
	ChangeUpdate();

	//Stage::Update();
}

void Stage::Update()
{
	//�V�[���؂�ւ����Ó]�܂œ��B������
	if (SceneChange::Get()->IsBlackOut())
	{
		//�؂�ւ��J�n(��������)
		ChangeUpdate();

		//�؂�ւ����I�������Ó]������
		SceneChange::Get()->Open();
	}

	for (auto& obj : mEntitys)
	{
		obj.Update(*Camera::sCamera);
		obj.box.Update(*Camera::sCamera);
		//obj.box.CreateCol(obj.position,);
	}
	for (auto& obj : mEventObjects)
	{
		obj->Update();
	}

	//StarManager::Get()->Update();

	for (auto& obj : CollideManager::Get()->allCols)
	{
		obj->Update(*Camera::sCamera);
	}
	for (auto& obj : mGoals)
	{
		obj->Update();
	}

	goalSystem.Update();
}

void Stage::Draw()
{
	//���ɂ���ă}�e���A���`��ƃe�N�X�`���`�悪���݂��Ă�̂�
	//��������@������ĂȂ��̂ō��
	BasicObjectPreDraw(PipelineManager::GetPipeLine("Toon"));
	DrawCollider();

	DrawModel();
}

void Stage::DrawSprite()
{
	goalSystem.Draw();
}

void Stage::Reload()
{
	std::string loadfile = "Scene/";
	loadfile += Stage::Get()->GetNowStageHandle();
	LevelLoader::Get()->Load(loadfile, Stage::Get()->GetNowStageHandle());

	Stage::Get()->ChangeLevel(*LevelLoader::Get()->GetData(Stage::Get()->GetNowStageHandle()));
}

std::string Stage::GetNowStageHandle()
{
	return currentHandle;
}

void Stage::NormalObjectSet(const LevelData::ObjectData& data)
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
	
	//�I�u�W�F�N�g�̔z�u
	LevelDataExchanger::SetObjectData(mEntitys.back(), data);
}

void Stage::CollisionSet(const LevelData::ObjectData& data)
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

void Stage::CollisionSetEvent(const LevelData::ObjectData& data)
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

void Stage::EvenyObjectSet(const LevelData::ObjectData& data)
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
	if (data.eventtrigerName.find("goal") != std::string::npos)
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

void Stage::ChangeUpdate()
{
	//�n���h�����X�e�[�W�ɕۑ�
	currentHandle = currentData->mHandle;

	//�����Ă����̂��폜
	mEntitys.clear();
	mEventObjects.clear();
	mGoals.clear();
	//StarManager::Get()->mStars.clear();
	CollideManager::Get()->allCols.clear();
	mCannonPoints.clear();
	EnemyManager::Get()->enemyList.clear();

	Player::Get()->Register();

	for (auto objectData = currentData->mObjects.begin(); objectData != currentData->mObjects.end(); objectData++)
	{
		//�v���C���[�̔z�u�Ȃ�
		if (objectData->setObjectName == "player")
		{
			SetPlayer(*objectData);

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

			//setObject�𕪉�����
			std::vector<std::string> split = Util::SplitString(objectData->setObjectName, "_");

			Dokan* dokan = dynamic_cast<Dokan*>(mEventObjects.back().get());
			for (auto str : split)
			{
				//�������������o��
				if (Util::IsNumber(str))
				{
					dokan->dokanInfo.id = str;
				}
				//stage�ȊO�̕�����Ȃ�
				else if (str != "dokan")
				{
					dokan->dokanInfo.stageName = str;
				}
			}

			//�v���C���[�̔z�u
			if (oldDokanInfo.stageName.find(dokan->dokanInfo.stageName) != std::string::npos &&
				oldDokanInfo.id.find(dokan->dokanInfo.id) != std::string::npos)
			{
				Player::Get()->Reset();
				Player::Get()->preMove = objectData->translation;
				Player::Get()->position = objectData->translation;
				Player::Get()->rotation = objectData->rotation;
			}

			continue;
		}

		if (objectData->setObjectName == "boardpicture")
		{
			//���̂܂܃��f���̔z�u
			NormalObjectSet(*objectData);

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

			//�����蔻����쐬
			if (objectData->collider.have)
			{
				CollisionSet(*objectData);
			}

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
		if (cannon == nullptr)continue;

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

		//TODO:���O�ɕۊǂ����f�[�^�Q����������ہA���������S����(���`�T��)���Ă���̂�
		//�f�[�^�̑��ʂ�������Ə������Ԃ��̂тĂ��܂� �A�z
		//�T������ς��Ȃ����j�ŉ��P����Ȃ�A���łɑS�Ẵf�[�^���������Ă���f�[�^��
		//�X�L�b�v����Ȃǂ̏������K�v���낤
	}
}

void Stage::SetPlayer(const LevelData::ObjectData& data)
{
	Player::Get()->Reset();
	Player::Get()->preMove = data.translation;
	Player::Get()->position = data.translation;
	Player::Get()->rotation = data.rotation;
	//Player::Get()->scale = data.scaling;
}

void Stage::DrawModel()
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
		obj.DrawMaterial();
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
	//for (auto& obj : StarManager::Get()->mStars)
	//{
	//	BasicObjectPreDraw(PipelineManager::GetPipeLine("OutLine"), false);
	//	obj->DrawOutLine();
	//	//�A���t�@��1�����ɂȂ�Ȃ瓧���p�`��p�C�v���C���ɐ؂�ւ���
	//	if (obj->color_.w < 1.0f)
	//	{
	//		BasicObjectPreDraw(PipelineManager::GetPipeLine("GroundToonNDW"));
	//	}
	//	else
	//	{
	//		BasicObjectPreDraw(PipelineManager::GetPipeLine("GroundToon"));
	//	}
	//	obj->Draw();
	//}

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
}

void Stage::DrawCollider()
{
	if (mShowCollider == false) return;
	for (auto& obj : mEntitys)
	{
		if (!obj.CheckTag(TagTable::Collsion))continue;

		obj.box.Draw();
	}
}
