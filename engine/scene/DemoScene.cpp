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
	mObject3dPipelineSet = CreateObject3DPipeline();

	mCamera->Initialize();

	mDebugCamera.Initialize();

	mSkydome.Initialize();
	mSkydome.SetModel(ModelManager::Get()->GetModel("skydome"));
	mSkydome.SetTexture(TextureManager::Get()->GetTexture("white"));
	mSkydome.scale = { 10,10,10 };
	mSkydome.color_ = { 1.f,1.f,1.f,1.0f };

	mCamera->mEye.y = 50;
	mCamera->mEye.z = -150;

	ModelManager::LoadModel("beetle","beetle");
	ModelManager::LoadModel("firewisp","firewisp");
	ModelManager::LoadModel("boss","boss");
	ModelManager::LoadModel("BlankCube","BlankCube");
	
	mSlime.SetTexture(*TextureManager::GetTexture("slime"));
	mSlime.mPosition = { 0,0,0 };

	LevelLoader::Get()->Load("Scene/worldTest_Children", "children");
	SetObject(*LevelLoader::Get()->GetData("children"));
}

void DemoScene::Update()
{
	if (Input::Keyboard::TriggerKey(DIK_O))
	{
		mSceneManager->ChangeScene<Game>();
		//sceneManager->ChangeScene("GAMEPLAY");
	}

	if (Input::Keyboard::TriggerKey(DIK_R))
	{
		mObj3ds.clear();

		LevelLoader::Get()->Load("Scene/worldTest_Children", "children");
		SetObject(*LevelLoader::Get()->GetData("children"));
	}

	mCamera->UpdatematView();

	mSkydome.Update(*mCamera);
	for (auto& obj : mObj3ds)
	{
		obj.Update(*mCamera);
	}

	mSlime.Update();

	mDebugCamera.Update();

	mGui.Begin({ 100,100 }, { 1,1 });
	mGui.End();
}

void DemoScene::Draw()
{
	BasicObjectPreDraw(mObject3dPipelineSet);

	mSkydome.DrawMaterial();

	for (auto& obj : mObj3ds)
	{
		obj.Draw();
	}

	//�X�v���C�g�̑O�`��(���ʃR�}���h)
	SpriteCommonBeginDraw(SpriteCommon::sSpriteCommon);

	mSlime.Draw();
}

void DemoScene::End()
{
	mObj3ds.clear();
}

void DemoScene::SetObject(LevelData& data)
{
	/*auto itr = particles.begin();
	itr != particles.end(); itr++*/
	for (auto objectData = data.mObjects.begin(); objectData != data.mObjects.end(); objectData++)
	{
		//�Ƃ肠�����L���[�u�Ŕz�u
		//TODO:file_name����t�����ł���悤�ɂ�����
		mObj3ds.emplace_back();
		mObj3ds.back().Initialize();
		//���f���ݒ�
		//�t�@�C���l�[�����ݒ肳��Ă�Ȃ炻���
		if (objectData->fileName != "")
		{
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
		mObj3ds.back().rotation = objectData->rotation;
		//�傫��
		mObj3ds.back().scale = objectData->scaling;

		//�����蔻����쐬
		if (objectData->collider.have)
		{
			mObj3ds.emplace_back();
			mObj3ds.back().Initialize();

			mObj3ds.back().SetModel(ModelManager::GetModel("BlankCube"));
			mObj3ds.back().SetTexture(TextureManager::Get()->GetTexture("white"));

			mObj3ds.back().position = objectData->translation;
			mObj3ds.back().scale = objectData->collider.size;
			mObj3ds.back().rotation = { 0,0,0 };
		}
	}
}
