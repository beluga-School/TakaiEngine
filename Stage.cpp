#include "Stage.h"
#include "Model.h"
#include "MathF.h"

void Stage::Load(LevelData& data)
{
	/*auto itr = particles.begin();
	itr != particles.end(); itr++*/
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
		if (objectData->spawnpointName == "enemy")
		{
			//�Ƃ肠�����L���[�u�Ŕz�u
			mObj3ds.emplace_back();
			mObj3ds.back().Initialize();

			mObj3ds.back().SetModel(ModelManager::GetModel("spawnpoint"));
			//���W
			mObj3ds.back().position = objectData->translation;
			//��]�p
			mObj3ds.back().rotation = {
					MathF::AngleConvRad(objectData->rotation.x),
					MathF::AngleConvRad(objectData->rotation.y),
					MathF::AngleConvRad(objectData->rotation.z)
			};
			//�傫��
			mObj3ds.back().scale = objectData->scaling;
		}
		else
		{
			//�Ƃ肠�����L���[�u�Ŕz�u
			//TODO:file_name����t�����ł���悤�ɂ�����
			mObj3ds.emplace_back();
			mObj3ds.back().Initialize();
			//���f���ݒ�
			//�t�@�C���l�[�����ݒ肳��Ă�Ȃ炻���
			if (objectData->fileName != "")
			{
				//�ǂݍ��݂��ĂȂ��Ȃ�ǂݍ��݂��s��
				if (ModelManager::GetModel(objectData->fileName) == nullptr)
				{
					ModelManager::LoadModel(objectData->fileName, objectData->fileName);
				}
				mObj3ds.back().SetModel(ModelManager::GetModel(objectData->fileName));
			}
			//�Ȃ��Ȃ�l�p���f�t�H�Őݒ�
			else
			{
				mObj3ds.back().SetModel(ModelManager::GetModel("Cube"));
			}
			mObj3ds.back().SetTexture(TextureManager::Get()->GetTexture("white"));
			//���W
			mObj3ds.back().position = objectData->translation;
			//��]�p
			mObj3ds.back().rotation = {
					MathF::AngleConvRad(objectData->rotation.x),
					MathF::AngleConvRad(objectData->rotation.y),
					MathF::AngleConvRad(objectData->rotation.z)
			};
			//�傫��
			mObj3ds.back().scale = objectData->scaling;

			//�����蔻����쐬
			if (objectData->collider.have)
			{
				//�����蔻���\������I�u�W�F�N�g
				mObj3ds.emplace_back();
				mObj3ds.back().Initialize();

				mObj3ds.back().SetModel(ModelManager::GetModel("BlankCube"));
				mObj3ds.back().SetTexture(TextureManager::Get()->GetTexture("white"));

				mObj3ds.back().position = objectData->translation + objectData->collider.center;
				mObj3ds.back().scale = {
					objectData->scaling.x * objectData->collider.size.x,
					objectData->scaling.y * objectData->collider.size.y,
					objectData->scaling.z * objectData->collider.size.z
				};
				mObj3ds.back().rotation = {
					MathF::AngleConvRad(objectData->rotation.x),
					MathF::AngleConvRad(objectData->rotation.y),
					MathF::AngleConvRad(objectData->rotation.z)
				};

				//�����蔻�莩�̂̏����쐬
				mColCubes.emplace_back();
				mColCubes.back().position = objectData->translation + objectData->collider.center;
				mColCubes.back().scale = {
					objectData->scaling.x * objectData->collider.size.x,
					objectData->scaling.y * objectData->collider.size.y,
					objectData->scaling.z * objectData->collider.size.z
				};
			}
		}
	}
}

void Stage::Update()
{
	for (auto &obj: mObj3ds)
	{
		obj.Update(*Camera::sCamera);
	}
}

void Stage::Draw()
{
	//���ɂ���ă}�e���A���`��ƃe�N�X�`���`�悪���݂��Ă�̂�
	//��������@������ĂȂ��̂ō��
	for (auto& obj : mObj3ds)
	{
		if (obj.MODEL->mSaveModelname == "BlankCube")
		{
		}
		obj.DrawMaterial();
	}
}
