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

	skydome.Initialize();
	skydome.SetModel(ModelManager::Get()->GetModel("skydome"));
	skydome.SetTexture(&TextureManager::Get()->white);
	skydome.scale = { 10,10,10 };
	skydome.color = { 1.f,1.f,1.f,1.0f };

	camera->eye.y = 50;
	camera->eye.z = -150;

	ModelManager::LoadModel("beetle","beetle");
	ModelManager::LoadModel("firewisp","firewisp");
	ModelManager::LoadModel("boss","boss");
	ModelManager::LoadModel("BlankCube","BlankCube");
	
	slime.SetTexture(TextureManager::GetTexture("slime"));
	slime.position = { 0,0,0 };

	LevelData* data = LevelLoader::Get()->Load("Scene/worldTest_Children");
	SetObject(data);
}

void DemoScene::Update()
{
	if (input->TriggerKey(DIK_O))
	{
		Game* game = new Game();
		sceneManager->SetScene(game);
		//sceneManager->ChangeScene("GAMEPLAY");
	}

	if (input->TriggerKey(DIK_R))
	{
		obj3ds.clear();
		SetObject(LevelLoader::Get()->Load("Scene/worldTest_Children"));
	}

	camera->DebugMove();
	camera->UpdatematView();

	skydome.Update(*camera);
	for (auto& obj : obj3ds)
	{
		obj.Update(*camera);
	}
	gui.Begin({ 100,100 }, { 10,10});

	ImGui::Text("getMousePos %f %f", Input::Mouse::GetPos().x, Input::Mouse::GetPos().y);
	ImGui::Text("getMouseVel %f %f", Input::Mouse::GetVelocity().x, Input::Mouse::GetVelocity().y);
	ImGui::Text("Click:L %d", Input::Mouse::Down(Click::LEFT));
	ImGui::Text("Click:R %d", Input::Mouse::Down(Click::RIGHT));
	ImGui::Text("Click:M %d", Input::Mouse::Down(Click::MIDDLE));
	ImGui::Text("Click:MB4 %d", Input::Mouse::Down(Click::MB4));
	ImGui::Text("Trigger:L %d", Input::Mouse::Triggered(Click::LEFT));
	ImGui::Text("Trigger:R %d", Input::Mouse::Triggered(Click::RIGHT));
	ImGui::Text("Trigger:M %d", Input::Mouse::Triggered(Click::MIDDLE));
	ImGui::Text("Trigger:MB4 %d", Input::Mouse::Triggered(Click::MB4));
	ImGui::Text("Release:L %d", Input::Mouse::Released(Click::LEFT));
	ImGui::Text("Release:R %d", Input::Mouse::Released(Click::RIGHT));
	ImGui::Text("Release:M %d", Input::Mouse::Released(Click::MIDDLE));
	ImGui::Text("Release:MB4 %d", Input::Mouse::Released(Click::MB4));

	gui.End();

	slime.Update();

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
	SpriteCommonBeginDraw(SpriteCommon::spriteCommon);

	slime.Draw();
}

void DemoScene::End()
{
	obj3ds.clear();
}

void DemoScene::SetObject(LevelData* data)
{
	/*auto itr = particles.begin();
	itr != particles.end(); itr++*/
	for (auto objectData = data->objects.begin(); objectData != data->objects.end(); objectData++)
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
		obj3ds.back().SetTexture(&TextureManager::Get()->white);
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
			obj3ds.back().SetTexture(&TextureManager::Get()->white);

			obj3ds.back().position = objectData->translation;
			obj3ds.back().scale = objectData->collider.size;
			obj3ds.back().rotation = { 0,0,0 };
		}
	}
}
