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
	std::unique_ptr<IScene> mCurrentscene = nullptr;

	//ファクトリーを使用したシーン切り替え(バグあり)
	//void ChangeScene(const std::string& sceneName);

	template <class NextScene> void ChangeScene()
	{
		mNextscene = std::make_unique<NextScene>();
	}

	void SetSceneFactory(std::unique_ptr<AbstractSceneFactory>& sceneFactory_) {
		mSceneFactory = std::move(sceneFactory_);
	}

	static SceneManager* Get()
	{
		static SceneManager instance;
		return &instance;
	}

private:
	std::unique_ptr<IScene> mNextscene = nullptr;
	std::unique_ptr<AbstractSceneFactory> mSceneFactory = nullptr;

	SceneManager(){};
	~SceneManager() {
		End();
	}
	SceneManager(const SceneManager& a) = delete;
	SceneManager& operator=(const SceneManager&) = delete;

};

