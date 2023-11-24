#pragma once
#include "IScene.h"
#include "AbstractSceneFactory.h"
#include <memory>

/*! SceneManager
	シーンを管理し、切り替えるクラス
*/
class SceneManager
{
public:
	void Update();
	void Draw();
	void End();

	//現在のシーン
	std::unique_ptr<IScene> mCurrentscene = nullptr;

	/// <summary>
	/// シーン切り替え
	/// </summary>
	/// <typeparam name="NextScene">切り替えるシーンのクラス</typeparam>
	template <class NextScene> void ChangeScene()
	{
		mNextscene = std::make_unique<NextScene>();
	}

	/// <summary>
	/// シーンファクトリーの設定(未使用)
	/// </summary>
	/// <param name="sceneFactory_"></param>
	void SetSceneFactory(std::unique_ptr<AbstractSceneFactory>& sceneFactory_) {
		mSceneFactory = std::move(sceneFactory_);
	}

	//インスタンス取得
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

