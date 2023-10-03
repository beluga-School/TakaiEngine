#include "SceneManager.h"
#include "SceneChange.h"
#include <cassert>
#include "InstantDrawer.h"

void SceneManager::Update()
{
	//���ߍ��񂾕`����s��j��
	InstantDrawer::DrawInit();

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
		
		//���\�[�X�ǂݍ���
		mCurrentscene->LoadResource();

		//���V�[����������
		mCurrentscene->Initialize();

		//�Ó]������
		if (SceneChange::Get()->IsBlackOut())
		{
			SceneChange::Get()->Open();
		}
	}

	//���s���V�[���̍X�V
	mCurrentscene->Update();

	SceneChange::Get()->Update();
}

void SceneManager::Draw()
{
	//���s���V�[���̕`��
	mCurrentscene->Draw();

	//�ȈՕ`��N���X�̖��t���[���X�V
	InstantDrawer::AllUpdate();
	//�ȈՕ`��N���X�̕`��
	InstantDrawer::AllDraw3D();
	InstantDrawer::AllDraw2D();

	//�؂�ւ����o�̕`��
	SpriteCommonBeginDraw();
	SceneChange::Get()->Draw();
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
