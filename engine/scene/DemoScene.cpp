#include "DemoScene.h"
#include <ClearDrawScreen.h>
#include <Sprite.h>
#include <SceneManager.h>
#include "GameScene.h"
#include "AssimpLoader.h"
#include <TimeManager.h>

void DemoScene::Initialize()
{
	//3d�I�u�W�F�N�g�p�̃p�C�v���C������
	object3dPipelineSet = CreateObject3DPipeline();

	camera->Initialize();

	cameraObject.Initialize();

	skydome.Initialize();
	skydome.SetModel(ModelManager::Get()->GetModel("skydome"));
	skydome.SetTexture(TextureManager::Get()->GetTexture("white"));
	skydome.scale = { 10,10,10 };
	skydome.color_ = { 1.f,1.f,1.f,1.0f };

	camera->mEye.y = 50;
	camera->mEye.z = -150;

	ModelManager::LoadModel("beetle","beetle");
	ModelManager::LoadModel("firewisp","firewisp");
	ModelManager::LoadModel("boss","boss");
	ModelManager::LoadModel("BlankCube","BlankCube");
	
	slime.SetTexture(*TextureManager::GetTexture("slime"));
	slime.mPosition = { 0,0,0 };

	LevelData* data = LevelLoader::Get()->Load("Scene/worldTest_Children");
	SetObject(*data);
}

void DemoScene::Update()
{
	if (input->TriggerKey(DIK_O))
	{
		mSceneManager->ChangeScene<Game>();
		//sceneManager->ChangeScene("GAMEPLAY");
	}

	if (input->TriggerKey(DIK_R))
	{
		obj3ds.clear();
		SetObject(*LevelLoader::Get()->Load("Scene/worldTest_Children"));
	}

	camera->UpdatematView();

	skydome.Update(*camera);
	for (auto& obj : obj3ds)
	{
		obj.Update(*camera);
	}

	slime.Update();

	cameraObject.Update();

	gui.Begin({ 100,100 }, { 1,1 });
	gui.End();
}

void DemoScene::Draw()
{
	BasicObjectPreDraw(object3dPipelineSet);

	skydome.DrawMaterial();

	for (auto& obj : obj3ds)
	{
		obj.Draw();
	}

	//�X�v���C�g�̑O�`��(���ʃR�}���h)
	SpriteCommonBeginDraw(SpriteCommon::mSpriteCommon);

	slime.Draw();
}

void DemoScene::End()
{
	obj3ds.clear();
}

void DemoScene::SetObject(LevelData& data)
{
	/*auto itr = particles.begin();
	itr != particles.end(); itr++*/
	for (auto objectData = data.objects.begin(); objectData != data.objects.end(); objectData++)
	{
		//�Ƃ肠�����L���[�u�Ŕz�u
		//TODO:file_name����t�����ł���悤�ɂ�����
		obj3ds.emplace_back();
		obj3ds.back().Initialize();
		//���f���ݒ�
		//�t�@�C���l�[�����ݒ肳��Ă�Ȃ炻���
		if (objectData->fileName != "")
		{
			obj3ds.back().SetModel(ModelManager::GetModel(objectData->fileName));
		}
		//�Ȃ��Ȃ�l�p���f�t�H�Őݒ�
		else
		{
			obj3ds.back().SetModel(ModelManager::GetModel("Cube"));
		}
		obj3ds.back().SetTexture(TextureManager::Get()->GetTexture("white"));
		//���W
		obj3ds.back().position = objectData->translation;
		//��]�p
		obj3ds.back().rotation = objectData->rotation;
		//�傫��
		obj3ds.back().scale = objectData->scaling;

		//�����蔻����쐬
		if (objectData->collider.have)
		{
			obj3ds.emplace_back();
			obj3ds.back().Initialize();

			obj3ds.back().SetModel(ModelManager::GetModel("BlankCube"));
			obj3ds.back().SetTexture(TextureManager::Get()->GetTexture("white"));

			obj3ds.back().position = objectData->translation;
			obj3ds.back().scale = objectData->collider.size;
			obj3ds.back().rotation = { 0,0,0 };
		}
	}
}
