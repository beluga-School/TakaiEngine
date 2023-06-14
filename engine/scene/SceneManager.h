#pragma once
#include "IScene.h"
#include "AbstractSceneFactory.h"
#include <memory>

class SceneManager
{
public:

	void Update();
	void Draw();

	void End();

	//現在のシーン
	std::unique_ptr<IScene> currentscene = nullptr;

	//ファクトリーを使用したシーン切り替え(バグあり)
	//void ChangeScene(const std::string& sceneName);

	template <class NextScene> void ChangeScene()
	{
		nextscene = std::make_unique<NextScene>();
	}

	void SetSceneFactory(std::unique_ptr<AbstractSceneFactory>& sceneFactory_) {
		sceneFactory = std::move(sceneFactory_);
	}

	static SceneManager* Get()
	{
		static SceneManager instance;
		return &instance;
	}

private:
	std::unique_ptr<IScene> nextscene = nullptr;
	std::unique_ptr<AbstractSceneFactory> sceneFactory = nullptr;

	SceneManager(){};
	~SceneManager() {
		End();
	}
	SceneManager(const SceneManager& a) = delete;
	SceneManager& operator=(const SceneManager&) = delete;

};

