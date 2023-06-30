#include "Stage.h"
#include "Model.h"
#include "MathF.h"
#include "EnemyManager.h"

void Stage::ChangeLevel(LevelData& data)
{
	//�����Ă����̂��폜
	mObj3ds.clear();
	mColCubes.clear();
	mEventObjects.clear();

	for (auto objectData = data.mObjects.begin(); objectData != data.mObjects.end(); objectData++)
	{
		////�X�|�[���|�C���g��ݒ�
		////�w�肳�ꂽ���O�������Ă�Ȃ�Ή������������
		//if (objectData->spawnpointName == "player")
		//{
		//	testplayer.position = objectData->translation;
		//	testplayer.rotation = objectData->rotation;
		//	testplayer.scale = objectData->scaling;
		//}
		
		//�G�l�~�[�Ȃ�
		//TODO:���}�I��eventtriger���g���Ă��邪�A����̂��Ƃ��l����ƓG�̔z�u��p�̍��ڂ��������������
		if (objectData->eventtrigerName == "enemy")
		{
			EnemyManager::Get()->Load(*objectData);
			continue;
		}

		//�C�x���g�I�u�W�F�N�g�Ȃ�
		if (objectData->eventtrigerName != "")
		{
			//�ݒu���Ďc����X�L�b�v
			EvenyObjectSet(*objectData);
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
		else
		{
			NormalObjectSet(*objectData);

			//�����蔻����쐬
			if (objectData->collider.have)
			{
				CollisionSet(*objectData);
			}

			continue;
		}
	}
}

void Stage::Update()
{
	for (auto &obj: mObj3ds)
	{
		obj.Update(*Camera::sCamera);
	}

	for (auto& obj : mEventObjects)
	{
		obj.Update();
	}

	for (auto& obj : mColObj3ds)
	{
		obj.Update(*Camera::sCamera);
	}
}

void Stage::Draw()
{
	//���ɂ���ă}�e���A���`��ƃe�N�X�`���`�悪���݂��Ă�̂�
	//��������@������ĂȂ��̂ō��
	DrawCollider();

	DrawModel();
}

void Stage::NormalObjectSet(const LevelData::ObjectData& data)
{
	//�Ƃ肠�����L���[�u�Ŕz�u
	mObj3ds.emplace_back();
	mObj3ds.back().Initialize();
	//���f���ݒ�
	//�t�@�C���l�[�����ݒ肳��Ă�Ȃ炻���
	if (data.fileName != "")
	{
		//�ǂݍ��݂��ĂȂ��Ȃ�ǂݍ��݂��s��
		if (ModelManager::GetModel(data.fileName) == nullptr)
		{
			ModelManager::LoadModel(data.fileName, data.fileName);
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
	mEventObjects.emplace_back();
	mEventObjects.back().Initialize();
	mEventObjects.back().trigerName = data.eventtrigerName;

	//�o�O��Ȃ��悤�ɔ��e�N�X�`��������
	mEventObjects.back().SetTexture(TextureManager::Get()->GetTexture("white"));
	
	//�I�u�W�F�N�g�̔z�u
	LevelDataExchanger::SetObjectData(mEventObjects.back(), data);

}

void Stage::DrawModel()
{
	if (mShowModel == false) return;
	for (auto& obj : mObj3ds)
	{
		obj.DrawMaterial();
	}
	for (auto& obj : mEventObjects)
	{
		obj.Draw();
	}
}

void Stage::DrawCollider()
{
	if (mShowCollider == false) return;
	for (auto& obj : mColObj3ds)
	{
		obj.DrawMaterial();
	}
}
