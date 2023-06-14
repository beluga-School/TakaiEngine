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
	
	void SetScene(IScene& nextScene_) {
		nextscene = &nextScene_;
	};

	void SetSceneFactory(const AbstractSceneFactory& sceneFactory_) {
		sceneFactory = &sceneFactory_;
	}

	static SceneManager* Get()
	{
		static SceneManager instance;
		return &instance;
	}

private:
	IScene* nextscene = nullptr;
	const AbstractSceneFactory* sceneFactory = nullptr;

	SceneManager(){};
	~SceneManager() {
		End();
	}
	SceneManager(const SceneManager& a) = delete;
	SceneManager& operator=(const SceneManager&) = delete;

};

