#pragma once
#include "IScene.h"
#include "AbstractSceneFactory.h"

class SceneManager
{
public:

	void Update();
	void Draw();

	void End();

	//���݂̃V�[��
	IScene* currentscene = nullptr;

	//�t�@�N�g���[���g�p�����V�[���؂�ւ�(�o�O����)
	//void ChangeScene(const std::string& sceneName);
	void SetScene(IScene* nextScene) {
		nextscene = nextScene;
	};

	void SetSceneFactory(AbstractSceneFactory *sceneFactory) {
		this->sceneFactory = sceneFactory;
	}

	static SceneManager* Get()
	{
		static SceneManager instance;
		return &instance;
	}

private:
	IScene* nextscene = nullptr;
	AbstractSceneFactory* sceneFactory = nullptr;

	SceneManager(){};
	~SceneManager() {
		End();
	}
	SceneManager(const SceneManager& a) = delete;
	SceneManager& operator=(const SceneManager&) = delete;

};

