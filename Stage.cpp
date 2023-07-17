#include "Stage.h"
#include "Model.h"
#include "MathF.h"
#include "EnemyManager.h"
#include "Player.h"
#include "WarpBlock.h"
#include "ClearDrawScreen.h"
#include "Star.h"
#include "SceneChange.h"

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

	for (auto &obj: mObj3ds)
	{
		obj.Update(*Camera::sCamera);
	}

	for (auto& obj : mEventObjects)
	{
		obj->Update();
	}

	for (auto& obj : StarManager::Get()->mStars)
	{
		obj->Update();
	}
	StarManager::Get()->Update();

	for (auto& obj : mColObj3ds)
	{
		obj.Update(*Camera::sCamera);
		obj.CreateCol();
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
	DrawCollider();

	DrawModel();
}

void Stage::DrawSprite()
{
	goalSystem.Draw();
}

std::string Stage::GetNowStageHandle()
{
	return currentHandle;
}

void Stage::NormalObjectSet(const LevelData::ObjectData& data)
{
	//�R���W�����ړI�Ŕz�u�����Ȃ�I�u�W�F�N�g�z�u���s��Ȃ�
	if (data.fileName == "collision")
	{
		return;
	}

	//�Ƃ肠�����L���[�u�Ŕz�u
	mObj3ds.emplace_back();
	mObj3ds.back().Initialize();

	//�A�E�g���C���ݒ�
	mObj3ds.back().SetOutLineState({ 1,0,0,1.0f }, 0.05f);

	//���f���ݒ�
	//�t�@�C���l�[�����ݒ肳��Ă�Ȃ炻���
	if (data.fileName != "")
	{
		//�ǂݍ��݂��ĂȂ��Ȃ�ǂݍ��݂��s��
		if (ModelManager::GetModel(data.fileName) == nullptr)
		{
			ModelManager::LoadModel(data.fileName, data.fileName,true);
		}
		mObj3ds.back().SetModel(ModelManager::GetModel(data.fileName));
	}
	//�Ȃ��Ȃ�l�p���f�t�H�Őݒ�
	else
	{
		mObj3ds.back().SetModel(ModelManager::GetModel("Cube"));
	}
	//�o�O��Ȃ��悤�ɔ��e�N�X�`��������
	mObj3ds.back().SetTexture(TextureManager::Get()->GetTexture("white"));
	
	//�I�u�W�F�N�g�̔z�u
	LevelDataExchanger::SetObjectData(mObj3ds.back(), data);
}

void Stage::CollisionSet(const LevelData::ObjectData& data)
{
	//�����蔻���\������I�u�W�F�N�g
	mColObj3ds.emplace_back();
	mColObj3ds.back().Initialize();

	mColObj3ds.back().SetModel(ModelManager::GetModel("BlankCube"));
	mColObj3ds.back().SetTexture(TextureManager::Get()->GetTexture("white"));

	mColObj3ds.back().position = data.translation + data.collider.center;
	mColObj3ds.back().scale = {
		data.scaling.x * data.collider.size.x,
		data.scaling.y * data.collider.size.y,
		data.scaling.z * data.collider.size.z
	};
	mColObj3ds.back().rotation = {
		MathF::AngleConvRad(data.rotation.x),
		MathF::AngleConvRad(data.rotation.y),
		MathF::AngleConvRad(data.rotation.z)
	};

	//�����蔻�莩�̂̏����쐬
	mColCubes.emplace_back();
	mColCubes.back().position = data.translation + data.collider.center;
	mColCubes.back().scale = {
		data.scaling.x * data.collider.size.x,
		data.scaling.y * data.collider.size.y,
		data.scaling.z * data.collider.size.z
	};
}

void Stage::EvenyObjectSet(const LevelData::ObjectData& data)
{
	//stage �̕����񂪊܂܂�Ă�Ȃ�
	if (data.eventtrigerName.find("stage") != std::string::npos)
	{
		mEventObjects.emplace_back();
		mEventObjects.back() = std::make_unique<WarpBlock>();
		mEventObjects.back()->Initialize();

		mEventObjects.back()->SetOutLineState({ 1,0,0,1.0f }, 0.05f);

		mEventObjects.back()->trigerName = data.eventtrigerName;
		//�o�O��Ȃ��悤�ɔ��e�N�X�`��������
		mEventObjects.back()->SetTexture(TextureManager::Get()->GetTexture("white"));

		//�I�u�W�F�N�g�̔z�u
		LevelDataExchanger::SetObjectData(*mEventObjects.back(), data);
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
	}
	//star �̕����񂪊܂܂�Ă�Ȃ�
	if (data.eventtrigerName.find("star") != std::string::npos)
	{
		StarManager::Get()->mStars.emplace_back();
		StarManager::Get()->mStars.back() = std::make_unique<Star>();
		StarManager::Get()->mStars.back()->Initialize();

		StarManager::Get()->mStars.back()->SetOutLineState({ 0,0,0,1.0f }, 0.1f);

		StarManager::Get()->mStars.back()->trigerName = data.eventtrigerName;
		
		//�I�u�W�F�N�g�̔z�u
		LevelDataExchanger::SetObjectData(*StarManager::Get()->mStars.back(), data);
	}
}

void Stage::ChangeUpdate()
{
	//�n���h�����X�e�[�W�ɕۑ�
	currentHandle = currentData->mHandle;

	//�����Ă����̂��폜
	mObj3ds.clear();
	mColCubes.clear();
	mEventObjects.clear();
	mColObj3ds.clear();
	mGoals.clear();
	StarManager::Get()->mStars.clear();
	CollideManager::Get()->allCols.clear();

	Player::Get()->Register();

	for (auto objectData = currentData->mObjects.begin(); objectData != currentData->mObjects.end(); objectData++)
	{
		//�v���C���[�̔z�u�Ȃ�
		if (objectData->setObjectName == "player")
		{
			Player::Get()->Reset();
			Player::Get()->preMove = objectData->translation;
			Player::Get()->position = objectData->translation;
			Player::Get()->rotation = objectData->rotation;
			Player::Get()->scale = objectData->scaling;

			continue;
		}

		//�G�l�~�[�̔z�u�Ȃ�
		if (objectData->setObjectName == "enemy")
		{
			EnemyManager::Get()->Load(*objectData);
			continue;
		}

		//�C�x���g�I�u�W�F�N�g�Ȃ�ݒu
		if (objectData->eventtrigerName != "")
		{
			//���ł���ɕ��ނ킯���Ĕz�u���Ă���
			//EventBlock �����N���X�ɁAHitEffect�̒��g��ς����N���X�Ŏ������Ă���
			EvenyObjectSet(*objectData);

			////�����蔻����쐬
			//if (objectData->collider.have)
			//{
			//	CollisionSet(*objectData);
			//}

			continue;
		}

		if (objectData->spawnpointName == "enemy")
		{
			//�Ƃ肠�����L���[�u�Ŕz�u
			mObj3ds.emplace_back();
			mObj3ds.back().Initialize();

			mObj3ds.back().SetModel(ModelManager::GetModel("spawnpoint"));

			LevelDataExchanger::SetObjectData(mObj3ds.back(), *objectData);

			continue;
		}

		//---�������O��continue�Y���ƁA���f����ǂݍ��񂶂���ăo�O��\����

		//�Ȃɂ�����������
		{
			//���̂܂܃��f���̔z�u
			NormalObjectSet(*objectData);

			//�����蔻����쐬
			if (objectData->collider.have)
			{
				CollisionSet(*objectData);
			
				//�R���W���������I�u�W�F�N�g�ւ̃|�C���^��ێ�
				mColObj3ds.back().collideObj = &mObj3ds.back();
			}

			continue;
		}
	}
}

void Stage::DrawModel()
{
	if (mShowModel == false) return;
	for (auto& obj : mObj3ds)
	{
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
	for (auto& obj : StarManager::Get()->mStars)
	{
		BasicObjectPreDraw(PipelineManager::GetPipeLine("OutLine"), false);
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
}

void Stage::DrawCollider()
{
	if (mShowCollider == false) return;
	for (auto& obj : mColObj3ds)
	{
		obj.Draw();
	}
}
