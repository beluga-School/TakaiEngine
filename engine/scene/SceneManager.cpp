#include "SceneManager.h"
#include <cassert>

void SceneManager::Update()
{
	//�؂�ւ��@�\
	//���̃V�[���̗\�񂪂���Ȃ�
	if (mNextscene)
	{
		//���V�[���̏I��
		if (mCurrentscene) {
			mCurrentscene->End();
		}

		//�؂�ւ�
		mCurrentscene.swap(mNextscene);
		mNextscene = nullptr;

		//�V�[���}�l�[�W�������݃V�[���ɃZ�b�g
		mCurrentscene->SetManager(*this);
		//���V�[����������
		mCurrentscene->Initialize();
	}

	//���s���V�[���̍X�V
	mCurrentscene->Update();
}

void SceneManager::Draw()
{
	//���s���V�[���̕`��
	mCurrentscene->Draw();
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
