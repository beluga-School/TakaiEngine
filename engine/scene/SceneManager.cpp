#include "SceneManager.h"
#include <cassert>

void SceneManager::Update()
{
	//�؂�ւ��@�\
	//���̃V�[���̗\�񂪂���Ȃ�
	if (nextscene)
	{
		//���V�[���̏I��
		if (currentscene) {
			currentscene->End();
		}

		//�؂�ւ�
		currentscene.swap(nextscene);
		nextscene = nullptr;

		//�V�[���}�l�[�W�������݃V�[���ɃZ�b�g
		currentscene->SetManager(*this);
		//���V�[����������
		currentscene->Initialize();
	}

	//���s���V�[���̍X�V
	currentscene->Update();
}

void SceneManager::Draw()
{
	//���s���V�[���̕`��
	currentscene->Draw();
}

void SceneManager::End()
{
	
}

//void SceneManager::ChangeScene(const std::string& sceneName)
//{
//	assert(sceneFactory);
//	assert(nextscene == nullptr);
//
//	//���̃V�[���𐶐�
//	nextscene = sceneFactory->CreateScene(sceneName);
//}
